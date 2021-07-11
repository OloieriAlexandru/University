const ObjectID = require('mongodb').ObjectID;

class CommentsRepository {
    constructor(mongoClient) {
        this.db = mongoClient.db('blog-app');
    }

    async getAllOnPost(postId) {
        try {
            return await this.db.collection('comments').find({
                postId: postId
            }).toArray();
        } catch {
            return null;
        }
    }

    async getByIdOnPost(postId, commentId) {
        try {
            return await this.db.collection('comments').findOne({
                _id: ObjectID(commentId),
                postId: postId
            });
        } catch {
            return null;
        }
    }

    async create(newComment) {
        try {
            let inserted = await this.db.collection('comments').insertOne(newComment);
            newComment._id = inserted.insertedId;
            return newComment;
        } catch {
            return null;
        }
    }

    async updateOnPost(postId, updatedPost) {
        let id = updatedPost._id;
        delete updatedPost._id;
        try {
            await this.db.collection('comments').updateOne({
                _id: ObjectID(id),
                postId: postId
            }, {
                $set: updatedPost
            });
        } catch {
            return null;
        }
    }

    async deleteOnPost(postId, commentId) {
        try {
            await this.db.collection('comments').deleteOne({
                _id: ObjectID(commentId),
                postId: postId
            });
        } catch (e) {
            return null;
        }
    }
}

module.exports = CommentsRepository;
