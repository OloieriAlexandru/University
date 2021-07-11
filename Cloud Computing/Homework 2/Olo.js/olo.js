const http = require('http');
const Router = require('./router.js');
const BadRequestResponse = require('./response').BadRequestResponse;

class Olo {
    constructor() {
        this.router = new Router();
    }

    method(method, path, handler, guards) {
        this.router.addRoute(method, path, handler, guards);
    }

    get(path, handler, guards) {
        this.method('GET', path, handler, guards);
    }

    post(path, handler, guards) {
        this.method('POST', path, handler, guards);
    }

    put(path, handler, guards) {
        this.method('PUT', path, handler, guards);
    }

    delete(path, handler, guards) {
        this.method('DELETE', path, handler, guards);
    }

    patch(path, handler, guards) {
        this.method('PATCH', path, handler, guards);
    }

    startServer(port, hostname, callback) {
        const router = this.router;

        http.createServer(function (req, res) {
            Olo.cors(req, res);

            let body = '';
            req.on('data', chunk => {
                body += chunk;
            });
            req.on('end', async () => {
                let response;
                try {
                    if (!Olo.validContentTypeHeader(body, req)) {
                        response = new BadRequestResponse({
                            message: `Invalid content-type header, expected application/json, got ${req.headers['content-type']}!`
                        });
                    } else {
                        if (body != '') {
                            body = JSON.parse(body);
                        }
                        response = await router.solve(req, res, body);
                        if (!response) {
                            response = new BadRequestResponse({
                                message: 'Route not found!'
                            });
                        }
                    }
                } catch (e) {
                    console.log(e);
                    response = new BadRequestResponse({
                        message: 'Invalid request body, expected valid JSON!'
                    });
                }
                console.log('------------RESPONSE----------------------');
                console.log(response);
                res.statusCode = response.statusCode;
                res.statusMessage = response.statusMessage;
                res.setHeader("Content-Type", "application/json");
                res.end(JSON.stringify(response.body));
            });
        }).listen(port, hostname);

        callback();
    }

    static validContentTypeHeader(body, req) {
        let methodsWithoutBody = ['GET', 'HEAD', 'OPTIONS', 'DELETE'];
        if (methodsWithoutBody.includes(req.method)) {
            return true;
        }
        return body != '' && req.headers['content-type'] && req.headers['content-type'] === 'application/json';
    }

    static cors(req, res) {
        res.setHeader('Access-Control-Allow-Origin', '*');
    }
}

module.exports = Olo;
