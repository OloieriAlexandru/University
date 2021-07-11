const http = require("http");
const config = require("./config");
const fs = require("fs");

const RouteController = require("./controller")["RouteController"];
const Route = require("./controller")["Route"];

controller = new RouteController([
    new Route("GET", "/api/v1/random-image", RouteController.getRandomImage),
    new Route("POST", "/api/v1/random-fact", RouteController.getRandomFact),
    new Route("POST", "/api/v1/fact-image", RouteController.getFactImage),
    new Route("GET", "/api/v1/metrics", RouteController.getMetrics),
]);

function setCorsOrigin(req, res) {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Request-Method", "*");
    res.setHeader("Access-Control-Allow-Methods", "OPTIONS, GET, POST");
    res.setHeader("Access-Control-Max-Age", 3600);
    res.setHeader(
        "Access-Control-Allow-Headers",
        "Content-Type, Access-Control-Allow-Headers"
    );
}

// https://stackoverflow.com/questions/11725691/how-to-get-a-microtime-in-node-js
function getCurrentMilliseconds() {
    let hrTime = process.hrtime();
    return hrTime[0] * 1000 + hrTime[1] / 1000000;
}

function addLogToFile(log) {
    return new Promise((resolve, reject) => {
        fs.appendFile(config.LOG_FILE_NAME, JSON.stringify(log) + '\n', function (err, data) {
            if (err) {
                reject(err);
            }
            resolve();
        });
    });
}

// https://stackoverflow.com/questions/6968448/where-is-body-in-a-nodejs-http-get-response
http
    .createServer(function (req, res) {
        setCorsOrigin(req, res);

        let data = "";

        req.on("data", (chunk) => {
            data += chunk;
        });
        req.on("end", async () => {
            let jsonBody = null;
            if (data != "") {
                try {
                    jsonBody = JSON.parse(data);
                } catch (e) {
                    res.statusCode = 400;
                    res.statusMessage = "Bad Request";
                    res.end("Invalid request body! Expected a valid JSON object!");
                    return;
                }
            }
            let startAt = getCurrentMilliseconds();
            let metricsObj = {};
            try {
                await controller.solve(req, res, jsonBody, metricsObj);
            } catch (err) {
                if (err && err["custom"] == 1) {
                    res.statusCode = err["statusCode"];
                    res.statusMessage = err["statusMessage"];
                    res.end(
                        JSON.stringify({
                            message: err["message"],
                        })
                    );
                } else {
                    res.statusCode = 500;
                    res.statusMessage = "Internal Server Error";
                    res.end(
                        JSON.stringify({
                            message: "Unexpected server error!",
                        })
                    );
                }
            } finally {
                let endAt = getCurrentMilliseconds();

                metricsObj["latency"] = endAt - startAt;
                metricsObj["statusCode"] = res.statusCode;

                try {
                    await addLogToFile(metricsObj);
                } catch (ignored) {}
            }
        });
    })
    .listen(config.PORT, config.HOST);

console.log(`Server listening at ${config.HOST}:${config.PORT}`);
