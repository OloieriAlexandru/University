const ObjectID = require('mongodb').ObjectID;

class PostsRepository {
    constructor(mongoClient) {
        this.db = mongoClient.db('blog-app');
    }

    async getAll() {
        return await this.db.collection('posts').find({}).toArray();
    }

    async getById(id) {
        try {
            return await this.db.collection('posts').findOne({
                _id: ObjectID(id)
            });
        } catch {
            return null;
        }
    }

    async create(newPost) {
        try {
            let inserted = await this.db.collection('posts').insertOne(newPost);
            newPost._id = inserted.insertedId;
            return newPost;
        } catch {
            return null;
        }
    }

    async update(updatedPost) {
        let id = updatedPost._id;
        delete updatedPost._id;
        try {
            await this.db.collection('posts').updateOne({
                _id: ObjectID(id)
            }, {
                $set: updatedPost
            });
        } catch {}
    }

    async delete(id) {
        try {
            await this.db.collection('posts').deleteOne({
                _id: ObjectID(id)
            });
        } catch {}
    }
}

module.exports = PostsRepository;
