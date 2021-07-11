const jwt = require('jsonwebtoken');
const crypto = require('crypto');
const config = require('../config.js');

// Taken from: https://github.com/CojocariuAlexandru/WEB/blob/master/server/users-microservice/server/jwt-utils.js
// (Written 100% by me)
class JwtUtil {
    // https://ciphertrick.com/salt-hash-passwords-using-nodejs-crypto/?fbclid=IwAR0QbqgLOHe-H3PJjmByQ5dukYrraNgH6_FB81ON_ryRqdXRn41MQChvFZ4
    static saltHashPassword(password) {
        let salt = JwtUtil.getPasswordSalt(16);
        return JwtUtil.sha512Password(password, salt);
    }

    static sha512Password(password, salt) {
        let hash = crypto.createHmac('sha512', salt);
        hash.update(password);
        let hashValue = hash.digest('hex').substr(0, 64);
        return {
            salt: salt,
            passwordHash: hashValue
        };
    }

    static getPasswordSalt(length) {
        return crypto.randomBytes(Math.ceil(length / 2))
            .toString('hex')
            .slice(0, length);
    }

    // https://ciphertrick.com/salt-hash-passwords-using-nodejs-crypto/?fbclid=IwAR0QbqgLOHe-H3PJjmByQ5dukYrraNgH6_FB81ON_ryRqdXRn41MQChvFZ4
    static guardIsLoggedIn(req, res) {
        let authorizationHeader = req.headers['authorization'];
        if (authorizationHeader == null || authorizationHeader.substr(0, 7) !== 'Bearer ') {
            return false;
        }

        let token = authorizationHeader.substr(7);
        try {
            let decoded = jwt.verify(token, config.ACCESS_TOKEN_SECRET);
            req.olo.userId = decoded._id;
            req.olo.userUsername = decoded.username;
        } catch (e) {
            return false;
        }

        return true;
    }
}

module.exports = JwtUtil;
