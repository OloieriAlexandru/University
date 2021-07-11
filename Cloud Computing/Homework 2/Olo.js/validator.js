const BadRequestResponse = require('./response').BadRequestResponse;

class PropertyValidator {
    constructor(propertyName) {
        this.required = true;
        this.propertyName = propertyName;
        this.type = null;
        this.minLength = null;
        this.maxLength = null;
    }

    isRequired(required) {
        this.required = required;
        return this;
    }

    isOfType(type) {
        this.type = type;
        return this;
    }

    hasLengthLimits(minLength, maxLength) {
        this.minLength = minLength;
        this.maxLength = maxLength;
        return this;
    }

    validate(object) {
        if (!object.hasOwnProperty(this.propertyName)) {
            if (this.required) {
                return new BadRequestResponse({
                    'message': `Invalid request body! Missing property ${this.propertyName}!`
                });
            } else {
                return true;
            }
        }
        let propValue = object[this.propertyName];
        let type = typeof propValue;
        if (this.type) {
            if (type != this.type) {
                return new BadRequestResponse({
                    'message': `Invalid request body! Property ${this.propertyName} is expected to be a "${this.type}", but is a "${type}"!`
                });
            }
        }
        if (type == 'string' && this.minLength != null && this.maxLength) {
            if (propValue.length < this.minLength) {
                return new BadRequestResponse({
                    'message': `Invalid request body! Property ${this.propertyName} has to have at least ${this.minLength} characters!`
                });
            }
            if (propValue.length > this.maxLength) {
                return new BadRequestResponse({
                    'message': `Invalid request body! Property ${this.propertyName} has not more than ${this.maxLength} characters!`
                });
            }
        }
        return true;
    }
}

class Validator {
    constructor() {
        this.properties = [];
    }

    addProperty(property) {
        this.properties.push(property);
        return this;
    }

    addProperties(properties) {
        this.properties.push(...properties);
        return this;
    }

    validate(object) {
        for (let property of this.properties) {
            let result = property.validate(object);
            if (result == true) {
                continue;
            }
            return result;
        }
        return true;
    }
}

module.exports = {
    PropertyValidator: PropertyValidator,
    Validator: Validator
};
