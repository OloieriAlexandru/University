const ObjectID = require('mongodb').ObjectID;

class UsersRepository {
    constructor(mongoClient) {
        this.db = mongoClient.db('blog-app');
    }

    async getById(id) {
        try {
            return await this.db.collection('users').findOne({
                _id: ObjectID(id)
            });
        } catch {
            return null;
        }
    }

    async getByUsername(username) {
        return await this.db.collection('users').findOne({
            username: username
        });
    }

    async create(newUser) {
        let inserted = await this.db.collection('users').insertOne(newUser);
        newUser._id = inserted.insertedId;
        return newUser;
    }

    async delete(id) {
        try {
            await this.db.collection('users').deleteOne({
                _id: ObjectID(id)
            });
        } catch {
            return null;
        }
    }
}

module.exports = UsersRepository;
