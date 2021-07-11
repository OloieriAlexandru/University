const ObjectID = require('mongodb').ObjectID;

class PostsService {
    constructor(postsRepository, commentsRepository) {
        this.repository = postsRepository;
        this.commentsRepository = commentsRepository;
    }

    async getAll() {
        return await this.repository.getAll();
    }

    async getById(id) {
        let post = await this.repository.getById(id);
        if (!post) {
            return post;
        }
        let postComments = await this.commentsRepository.getAllOnPost(id);
        post.comments = postComments;
        return post;
    }

    async create(newPost) {
        return await this.repository.create(newPost);
    }

    async update(updatedPost) {
        await this.repository.update(updatedPost);
    }

    async delete(id) {
        await this.repository.delete(id);
    }

    async getAllComments(postId) {
        return await this.commentsRepository.getAllOnPost(postId);
    }

    async getCommentById(postId, commentId) {
        return await this.commentsRepository.getByIdOnPost(postId, commentId);
    }

    async createComment(createdComment) {
        return await this.commentsRepository.create(createdComment);
    }

    async updateComment(postId, updatedComment) {
        await this.commentsRepository.updateOnPost(postId, updatedComment);
    }

    async updateComments(postId, newComments) {
        await this.deleteAllComments(postId);
        for (let i = 0; i < newComments.length; ++i) {
            newComments[i].postId = postId;
            try {
                newComments[i]._id = ObjectID(newComments[i]._id);
                let result = await this.createComment(newComments[i]);
                if (result == null) {
                    return null;
                }
            } catch (e) {
                console.log(e);
                return null;
            }
        }
        return true;
    }

    async deleteAllComments(postId) {
        let comments = await this.getAllComments(postId);
        for (let i = 0; i < comments.length; ++i) {
            try {
                await this.deleteComment(postId, comments[i]._id);
            } catch {}
        }
    }

    async deleteComment(postId, commentId) {
        await this.commentsRepository.deleteOnPost(postId, commentId);
    }
}

module.exports = PostsService;
