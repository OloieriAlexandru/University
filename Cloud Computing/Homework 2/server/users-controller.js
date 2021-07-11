const OkResponse = require('../Olo.js').OkResponse;
const CreatedResponse = require('../Olo.js').CreatedResponse;
const NoContentResponse = require('../olo.js').NoContentResponse;
const BadRequestResponse = require('../Olo.js').BadRequestResponse;

const PropertyValidator = require('../Olo.js').PropertyValidator;
const Validator = require('../Olo.js').Validator;

class UsersController {
    constructor(usersService) {
        this.service = usersService;
    }

    create = async (req, res) => {
        let result = this.validateCredentials(req.olo.body);
        if (result != true) {
            return result;
        }

        let newUser = {
            username: req.olo.body['username'],
            password: req.olo.body['password']
        };

        newUser = await this.service.create(newUser);
        if (!newUser) {
            return new BadRequestResponse({
                message: 'A user with the specified username already exists!'
            });
        }
        return new CreatedResponse(newUser);
    }

    login = async (req, res) => {
        let result = this.validateCredentials(req.olo.body);
        if (result != true) {
            return result;
        }
        let token = await this.service.getLoginToken(req.olo.body);
        if (!token) {
            return new BadRequestResponse({
                message: 'Invalid credentials!'
            });
        }
        return new OkResponse({
            token: token
        });
    }

    delete = async (req, res) => {
        await this.service.delete(req.olo.userId);
        return new NoContentResponse();
    }

    validateCredentials(body) {
        let username = new PropertyValidator('username').isOfType('string').hasLengthLimits(6, 32);
        let password = new PropertyValidator('password').isOfType('string').hasLengthLimits(6, 64);
        return new Validator().addProperties([username, password]).validate(body);
    }
}

module.exports = UsersController;
