const https = require("follow-redirects").https;
const requestLib = require("request").defaults({
    encoding: null,
});
const config = require("./config");
const fs = require('fs');

const SOME_RANDOM_API_DOWN = true;

const URL_RANDOM_IMAGE = "https://api.thedogapi.com/v1/images/search";

var URL_RANDOM_FACT;
if (SOME_RANDOM_API_DOWN) {
    URL_RANDOM_FACT = "https://dog-api.kinduff.com/api/facts";
} else {
    URL_RANDOM_FACT = "https://some-random-api.ml/facts/dog";
}

const URL_CREATE_FACT_IMAGE = "https://memegen.link/custom";
const URL_TRANSLATE =
    "https://translation.googleapis.com/language/translate/v2";
const NUMBER_OF_RETRIES = 10;

const ALLOWED_LANGUAGES = ["en", "fr", "es", "ro"];

class Route {
    constructor(method, path, handler) {
        this.method = method;
        this.path = path;
        this.handler = handler;
    }
}

function badRequest(message) {
    return {
        custom: 1,
        statusCode: 400,
        statusMessage: "Bad Request",
        message: message,
    };
}

function notFound(message) {
    return {
        custom: 1,
        statusCode: 404,
        statusMessage: "Not Found",
        message: message
    };
}

function internalServerError(message) {
    return {
        custom: 1,
        statusCode: 500,
        statusMessage: "Internal Server Error",
        message: message,
    };
}

// https://www.twilio.com/blog/2017/08/http-requests-in-node-js.html
// https://stackoverflow.com/questions/41470296/how-to-await-and-return-the-result-of-a-http-request-so-that-multiple-request
function makeRequest(url, options, body) {
    return new Promise((resolve, reject) => {
        let req = https.request(url, options, (res) => {
            let data = "";
            res.on("data", (chunk) => {
                data += chunk;
            });
            res.on("end", () => {
                resolve(data);
            });
            res.on("error", (err) => {
                reject(err);
            });
        });
        req.on("error", (err) => {
            reject(err);
        });
        if (body != null) {
            req.write(body);
        }
        req.end();
    });
}

// https://stackoverflow.com/questions/17124053/node-js-get-image-from-web-and-encode-with-base64
function downloadImage(url) {
    return new Promise((resolve, reject) => {
        requestLib.get(url, function (error, response, body) {
            if (error) {
                reject(error);
            }
            if (response.statusCode == 200) {
                data = "data:" + response.headers["content-type"] + ";base64," +
                    Buffer.from(body).toString("base64");
                resolve(data);
            }
        });
    });
}

function makeSuccessResponse(req, res, obj) {
    res.statusCode = 200;
    res.statusMessage = "Ok";
    res.setHeader("Content-Type", "application/json");
    return res.end(JSON.stringify(obj));
}

// https://stackoverflow.com/questions/11725691/how-to-get-a-microtime-in-node-js
function getCurrentMilliseconds() {
    let hrTime = process.hrtime();
    return hrTime[0] * 1000 + hrTime[1] / 1000000;
}

function addExternalAPICallToMetrics(metricsObj, apiName, latency) {
    if (!metricsObj.hasOwnProperty('apiMetrics')) {
        metricsObj.apiMetrics = {};
    }
    metricsObj.apiMetrics[apiName] = latency;
}

async function getRandomImage(metricsObj) {
    let getRandomImageOptions = {
        headers: {
            "x-api-key": config.THE_DOG_API_API_KEY,
        },
        method: "GET",
    };

    let startAt = getCurrentMilliseconds();
    let jsonResult = null;
    for (let retryIndex = 1; retryIndex <= NUMBER_OF_RETRIES; ++retryIndex) {
        try {
            let rawResult = await makeRequest(
                URL_RANDOM_IMAGE,
                getRandomImageOptions
            );
            jsonResult = JSON.parse(rawResult);
            break;
        } catch (ignored) {}
    }
    let endAt = getCurrentMilliseconds();

    if (jsonResult == null || jsonResult.length == 0) {
        throw internalServerError("Failed to get random image!");
    }

    addExternalAPICallToMetrics(metricsObj, 'random-image', endAt - startAt);
    return jsonResult[0]["url"];
}

async function getRandomFact(metricsObj) {
    let getRandomFactOptions = {
        method: "GET",
    };

    let startAt = getCurrentMilliseconds();
    let jsonResult = null;
    for (let retryIndex = 1; retryIndex <= NUMBER_OF_RETRIES; ++retryIndex) {
        try {
            let rawResult = await makeRequest(URL_RANDOM_FACT, getRandomFactOptions);
            jsonResult = JSON.parse(rawResult);
            break;
        } catch (ignored) {}
    }
    let endAt = getCurrentMilliseconds();

    if (jsonResult == null) {
        throw internalServerError("Failed to get random fact!");
    }

    addExternalAPICallToMetrics(metricsObj, 'random-fact', endAt - startAt);
    if (SOME_RANDOM_API_DOWN) {
        return jsonResult['facts'][0];
    }
    return jsonResult['fact'];
}

async function translate(text, language, metricsObj) {
    let translationOptions = {
        method: "POST",
    };
    let translationBody = {
        q: text,
        target: language
    };

    let startAt = getCurrentMilliseconds();
    let response = null;
    try {
        response = JSON.parse(
            await makeRequest(
                `${URL_TRANSLATE}?key=${config.TRANSLATE_API_KEY}`,
                translationOptions,
                JSON.stringify(translationBody)
            )
        );
    } catch (ignored) {}
    let endAt = getCurrentMilliseconds();

    if (!response || !response["data"] || !response["data"]["translations"] ||
        response["data"]["translations"].length == 0 || !response["data"]["translations"][0]["translatedText"]) {
        throw internalServerError("Failed to translate text!");
    }

    addExternalAPICallToMetrics(metricsObj, 'translate', endAt - startAt);
    return response["data"]["translations"][0]["translatedText"];
}

// https://stackoverflow.com/questions/36122391/parsing-json-data-without-a-comma-separator
function readLogsJSON() {
    return new Promise((resolve, reject) => {
        fs.readFile(config.LOG_FILE_NAME, function (err, data) {
            if (err) {
                reject(err);
            }
            try {
                resolve(Buffer.from(data).toString().split('\n').filter(el => el.length > 0).map(function (record) {
                    return JSON.parse(record);
                }));
            } catch (error) {
                reject(error);
            }
        });
    });
}

function pathWithLanguageInResponse(path) {
    return path === '/api/v1/random-fact' || path === '/api/v1/fact-image';
}

class RouteController {
    constructor(routes) {
        this.routes = routes;
    }

    static addRequestToMetrics(metricsObj, req, body) {
        metricsObj["request"] = {
            path: req.url,
            method: req.method,
            body: body,
        };
    }

    static addResponseToMetrics(metricsObj, obj) {
        metricsObj["response"] = Object.assign({}, obj);
    }

    // https://blog.risingstack.com/measuring-http-timings-node-js/
    async solve(req, res, body, metricsObj) {
        if (req.method === "OPTIONS") {
            res.statusCode = 200;
            return res.end();
        }
        RouteController.addRequestToMetrics(metricsObj, req, body);
        for (let route of this.routes) {
            if (req.method && req.url && req.method === route.method && req.url === route.path) {
                return route.handler(req, res, body, metricsObj);
            }
        }
        throw notFound("Invalid route path!");
    }

    static async getRandomImage(req, res, body, metricsObj) {
        let response = {
            url: await getRandomImage(metricsObj),
        };
        RouteController.addResponseToMetrics(metricsObj, response);

        return makeSuccessResponse(req, res, response);
    }

    static async getRandomFact(req, res, body, metricsObj) {
        if (!body || !body.hasOwnProperty("lang") || !ALLOWED_LANGUAGES.includes(body["lang"])) {
            throw badRequest(
                'Expected the body of the request to have an attribute called "lang"'
            );
        }

        let randomFact = await getRandomFact(metricsObj);
        randomFact = await translate(randomFact, body["lang"], metricsObj);

        let response = {
            fact: randomFact,
            lang: body["lang"]
        };
        RouteController.addResponseToMetrics(metricsObj, response);

        return makeSuccessResponse(req, res, response);
    }

    static async getFactImage(req, res, body, metricsObj) {
        let imageUrl = null;
        let imageCaption = null;
        let language = null;

        if (body && body.hasOwnProperty("url") && body["url"]) {
            imageUrl = body["url"];
        } else {
            imageUrl = await getRandomImage(metricsObj);
        }

        if (body && body.hasOwnProperty("fact") && body["fact"]) {
            imageCaption = body["fact"];
        } else {
            imageCaption = await getRandomFact(metricsObj);
        }
        if (body && body.hasOwnProperty("lang") && ALLOWED_LANGUAGES.includes(body["lang"])) {
            imageCaption = await translate(imageCaption, body["lang"], metricsObj);
            language = body["lang"];
        } else {
            language = "en";
        }
        let originalImageCaption = imageCaption;
        imageCaption = encodeURI(imageCaption);

        let factImageUrl = `${URL_CREATE_FACT_IMAGE}/${imageCaption}.jpg?alt=${imageUrl}`;
        let imageBase64Content = await downloadImage(factImageUrl);

        let response = {
            image: "base64Str",
            randomImage: imageUrl,
            fact: originalImageCaption,
            lang: language
        };
        RouteController.addResponseToMetrics(metricsObj, response);

        response["image"] = imageBase64Content;
        return makeSuccessResponse(req, res, response);
    }

    static async getMetrics(req, res, body, metricsObj) {
        let logs = null;
        try {
            logs = await readLogsJSON();
        } catch (err) {
            throw internalServerError("Failed to read logs file from disk!");
        }

        let response = {
            requestsCount: logs.length,
            averageLatency: 0.0,
            maxLatency: Number.MIN_SAFE_INTEGER,
            minLatency: Number.MAX_SAFE_INTEGER,
            averageLatencyForPath: {},
            maxLatencyForPath: {},
            minLatencyForPath: {},
            requestCountForPath: {},
            successfulCount: 0,
            requestPaths: {},
            requestMethods: {},
            languages: {},
            externalAPILatencies: {},
            externalAPILatenciesCount: {}
        };

        for (let i = 0; i < logs.length; ++i) {
            response.averageLatency += logs[i].latency;

            response.maxLatency = Math.max(response.maxLatency, logs[i].latency);
            response.minLatency = Math.min(response.minLatency, logs[i].latency);

            if (logs[i].statusCode >= 200 && logs[i].statusCode < 300) {
                ++response.successfulCount;
            }

            if (logs[i].request && logs[i].request.path) {
                let path = logs[i].request.path;
                if (path in response.averageLatencyForPath) {
                    response.maxLatencyForPath[path] = Math.max(response.maxLatencyForPath[path], logs[i].latency);
                    response.minLatencyForPath[path] = Math.min(response.minLatencyForPath[path], logs[i].latency);
                    response.averageLatencyForPath[path] += logs[i].latency;
                    ++response.requestCountForPath[path];
                } else {
                    response.maxLatencyForPath[path] = logs[i].latency;
                    response.minLatencyForPath[path] = logs[i].latency;
                    response.averageLatencyForPath[path] = logs[i].latency;
                    response.requestCountForPath[path] = 1;
                }
            }

            if (logs[i].request && logs[i].request.path) {
                if (logs[i].request.path in response.requestPaths) {
                    ++response.requestPaths[logs[i].request.path];
                } else {
                    response.requestPaths[logs[i].request.path] = 1;
                }
            }

            if (logs[i].request && logs[i].request.method) {
                if (logs[i].request.method in response.requestMethods) {
                    ++response.requestMethods[logs[i].request.method];
                } else {
                    response.requestMethods[logs[i].request.method] = 1;
                }
            }

            if (logs[i].request && logs[i].request.path) {
                if (pathWithLanguageInResponse(logs[i].request.path)) {
                    if (logs[i].response.lang in response.languages) {
                        ++response.languages[logs[i].response.lang];
                    } else {
                        response.languages[logs[i].response.lang] = 1;
                    }
                }
            }

            if (logs[i].apiMetrics) {
                Object.keys(logs[i].apiMetrics).forEach(function (key, index) {
                    if (key in response.externalAPILatencies) {
                        response.externalAPILatencies[key] += logs[i].apiMetrics[key];
                        ++response.externalAPILatenciesCount[key];
                    } else {
                        response.externalAPILatencies[key] = logs[i].apiMetrics[key];
                        response.externalAPILatenciesCount[key] = 1;
                    }
                });
            }
        }
        response.averageLatency /= logs.length;
        Object.keys(response.externalAPILatencies).forEach(function (key, index) {
            response.externalAPILatencies[key] /= response.externalAPILatenciesCount[key];
        });
        Object.keys(response.averageLatencyForPath).forEach(function (key, index) {
            response.averageLatencyForPath[key] /= response.requestCountForPath[key];
        });

        RouteController.addResponseToMetrics(metricsObj, response);
        return makeSuccessResponse(req, res, response);
    }
}

module.exports = {
    Route,
    RouteController,
};
