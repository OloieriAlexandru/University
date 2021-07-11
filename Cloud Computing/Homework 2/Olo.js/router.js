const InternalServerErrorResponse = require('./response').InternalServerErrorResponse;

const regexVars = ['string', 'num', 'alphanum'];
const regexVarsReplacements = ['[-a-zA-Z]+', '[0-9]+', '[-a-zA-Z0-9]+'];
const validRoutesRE = new RegExp(`(^(\/([-a-zA-Z0-9]+|\{[-a-zA-Z]+\:((${regexVars[0]})|(${regexVars[1]})|(${regexVars[2]}))\}))+$)|(^\/$)`, 'g');

class Route {
    constructor(method, path, handler, guards) {
        this.method = method;
        this.handler = handler;
        this.path = path;
        this.guards = guards;
        this.buildRegexInfo(this.path);
    }

    buildRegexInfo(path) {
        let match = path.match(validRoutesRE);
        if (!match) {
            throw `Invalid path: \"${path}\"`;
        }
        let pathParts = path.split('/');
        let regexBuildParts = [];
        let groupIdx = 1;
        this.pathRegexVarsMap = {};
        for (let pathPart of pathParts) {
            if (pathPart.startsWith('{') && pathPart.endsWith('}')) {
                pathPart = pathPart.substring(1, pathPart.length - 1);
                let parParts = pathPart.split(':');
                for (let i = 0; i < regexVars.length; ++i) {
                    if (parParts[1] === regexVars[i]) {
                        pathPart = `(${regexVarsReplacements[i]})`;
                        break;
                    }
                }
                this.pathRegexVarsMap[parParts[0]] = groupIdx++;
            }
            regexBuildParts.push(pathPart);
        }
        this.pathRegex = `^${regexBuildParts.join('/')}$`;
        this.pathRegexObj = RegExp(this.pathRegex);
    }
}

class Router {
    constructor() {
        this.routes = [];
    }

    addRoute(httpMethod, path, handler, guards) {
        this.routes.push(new Route(httpMethod, path, handler, guards));
    }

    // https://stackoverflow.com/questions/8312459/iterate-through-object-properties
    async solve(req, res, body) {
        try {
            for (let route of this.routes) {
                if (route.method !== req.method) {
                    continue;
                }
                if (!req.url.match(route.pathRegex)) {
                    continue;
                }
                let oloJs = {
                    body: body,
                    urlVars: {}
                };
                let urlParts = route.pathRegexObj.exec(req.url);
                // console.log(urlParts);
                Object.keys(route.pathRegexVarsMap).forEach((regexVar) => {
                    oloJs.urlVars[regexVar] = urlParts[route.pathRegexVarsMap[regexVar]];
                });
                req.olo = oloJs;
                if (route.guards) {
                    for (let guard of route.guards) {
                        let guardRes = await guard(req, res);
                        if (guardRes == true) {
                            continue;
                        }
                        return guardRes;
                    }
                }
                return await route.handler(req, res);
            }
        } catch (e) {
            console.log(e);
            return new InternalServerErrorResponse({
                'message': 'Unexpected server error occured!'
            });
        }
        return null;
    }
}

module.exports = Router;
