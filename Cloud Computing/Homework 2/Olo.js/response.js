class GenericResponse {
    constructor(statusCode, statusMessage, body) {
        this.statusCode = statusCode;
        this.statusMessage = statusMessage;
        this.body = body;
    }
}

class BadRequestResponse extends GenericResponse {
    constructor(body) {
        super('400', 'Bad Request', body);
    }
}

class UnauthorizedResponse extends GenericResponse {
    constructor(body) {
        super('401', 'Unauthorized', body);
    }
}

class ForbiddenResponse extends GenericResponse {
    constructor() {
        super('403', 'Forbidden', {
            message: 'You are not allowed to do this!'
        });
    }
}

class NotFoundResponse extends GenericResponse {
    constructor(body) {
        super('404', 'Not found', body);
    }
}

class InternalServerErrorResponse extends GenericResponse {
    constructor(body) {
        super('500', 'Internal Server Error', body);
    }
}

class OkResponse extends GenericResponse {
    constructor(body) {
        super('200', 'Ok', body);
    }
}

class CreatedResponse extends GenericResponse {
    constructor(body) {
        super('201', 'Created', body);
    }
}

class NoContentResponse extends GenericResponse {
    constructor() {
        super('204', 'No Content');
    }
}

class MethodNotAllowedResponse extends GenericResponse {
    constructor() {
        super('405', 'Method Not Allowed');
    }
}

class NotImplementedResponse extends GenericResponse {
    constructor() {
        super('501', 'Not Implemented');
    }
}

module.exports = {
    BadRequestResponse,
    UnauthorizedResponse,
    ForbiddenResponse,
    NotFoundResponse,
    OkResponse,
    CreatedResponse,
    NoContentResponse,
    InternalServerErrorResponse,
    MethodNotAllowedResponse,
    NotImplementedResponse
};
