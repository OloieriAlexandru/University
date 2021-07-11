const jwt = require('jsonwebtoken');
const JwtUtil = require('./jwt-util');
const config = require('../config');

class UsersService {
    constructor(usersRepository) {
        this.repository = usersRepository;
    }

    async create(newUser) {
        let user = await this.repository.getByUsername(newUser.username);
        if (user != null) {
            return null;
        }

        let passwordInfo = JwtUtil.saltHashPassword(newUser.password);
        newUser.password = passwordInfo.passwordHash;
        newUser.passwordSalt = passwordInfo.salt;
        user = await this.repository.create(newUser);
        delete user.password;
        delete user.passwordSalt;
        return user;
    }

    async getLoginToken(loginDetails) {
        let user = await this.repository.getByUsername(loginDetails.username);
        if (user == null) {
            return null;
        }

        let hashedInputPassword = JwtUtil.sha512Password(loginDetails.password, user.passwordSalt).passwordHash;

        if (hashedInputPassword !== user.password) {
            return null;
        }
        let tokenBody = {
            _id: user._id,
            username: user.username
        };
        let token = jwt.sign(tokenBody, config.ACCESS_TOKEN_SECRET, {
            expiresIn: '365d'
        });
        return token;
    }

    async delete(id) {
        await this.repository.delete(id);
    }
}

module.exports = UsersService;
