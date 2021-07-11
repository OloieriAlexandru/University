const OkResponse = require('../Olo.js').OkResponse;
const NoContentResponse = require('../Olo.js').NoContentResponse;
const NotFoundResponse = require('../Olo.js').NotFoundResponse;
const BadRequestResponse = require('../Olo.js').BadRequestResponse;
const MethodNotAllowedResponse = require('../Olo.js').MethodNotAllowedResponse;
const NotImplementedResponse = require('../Olo.js').NotImplementedResponse;

const PropertyValidator = require('../Olo.js').PropertyValidator;
const Validator = require('../Olo.js').Validator;

class PostsController {
    constructor(postsService, userRepository) {
        this.service = postsService;
        this.userRepository = userRepository;
    }

    getAll = async (req, res) => {
        return new OkResponse(await this.service.getAll());
    }

    getById = async (req, res) => {
        let post = await this.service.getById(req.olo.urlVars['id']);

        if (post == null) {
            return new NotFoundResponse({
                'message': `A post with id "${req.olo.urlVars['id']}" doesn't exist!`
            });
        }

        return new OkResponse(post);
    }

    create = async (req, res) => {
        let result = this.validatePost(req.olo.body, false);
        if (result != true) {
            return result;
        }

        let createdPost = {
            'title': req.olo.body['title'],
            'content': req.olo.body['content'],
            'userId': req.olo.userId
        };

        return new OkResponse(await this.service.create(createdPost));
    }

    createIdSpecified = async (req, res) => {
        return new MethodNotAllowedResponse();
    }

    update = async (req, res) => {
        let result = this.validatePost(req.olo.body, true);
        if (result != true) {
            return result;
        }

        let updatedPost = {
            '_id': req.olo.body['_id'],
            'title': req.olo.body['title'],
            'content': req.olo.body['content']
        };
        await this.service.update(updatedPost);

        return new NoContentResponse();
    }

    patch = async (req, res) => {
        return new NotImplementedResponse({
            'message': 'This method is not implemented yet!'
        });
    }

    delete = async (req, res) => {
        await this.service.delete(req.olo.urlVars['id']);
        return new NoContentResponse();
    }

    getAllComments = async (req, res) => {
        return new OkResponse(await this.service.getAllComments(req.olo.urlVars['id']));
    }

    getCommentById = async (req, res) => {
        let comment = await this.service.getCommentById(req.olo.urlVars['id'], req.olo.urlVars['commentId']);

        if (comment == null) {
            return new NotFoundResponse({
                'message': `A comment with id "${req.olo.urlVars['commentId']}" doesn't exist!`
            });
        }

        return new OkResponse(comment);
    }

    createComment = async (req, res) => {
        let result = this.validateComment(req.olo.body, false, false, false);
        if (result != true) {
            return result;
        }

        let createdComment = {
            'content': req.olo.body['content'],
            'postId': req.olo.urlVars['id'],
            'userId': req.olo.userId
        };

        return new OkResponse(await this.service.createComment(createdComment));
    }

    updateComment = async (req, res) => {
        let result = this.validateComment(req.olo.body, true, true, true);
        if (result != true) {
            return result;
        }

        let user = await this.userRepository.getById(req.olo.body['userId']);
        if (!user) {
            return new BadRequestResponse({
                'message': `Invalid request body! Invalid property value for \"userId\"!`
            });
        }

        let updatedComment = {
            '_id': req.olo.body['_id'],
            'content': req.olo.body['content'],
            'userId': req.olo.body['userId']
        };
        await this.service.updateComment(req.olo.urlVars['id'], updatedComment);
        updatedComment._id = req.olo.body['_id'];

        return new NoContentResponse();
    }

    updateComments = async (req, res) => {
        let result = this.validateCommentsArray(req.olo.body);
        if (result != true) {
            return result;
        }

        result = await this.service.updateComments(req.olo.urlVars['id'], req.olo.body);
        if (result == null) {
            return new BadRequestResponse({
                'message': 'Invalid request body! Could not create the comments, probably there are duplicate id\'s'
            });
        }

        return new NoContentResponse();
    }

    deleteComments = async (req, res) => {
        let postId = req.olo.urlVars['id'];
        await this.service.deleteAllComments(postId);
        return new NoContentResponse();
    }

    deleteComment = async (req, res) => {
        await this.service.deleteComment(req.olo.urlVars['id'], req.olo.urlVars['commentId']);
        return new NoContentResponse();
    }

    validatePost(body, idRequired) {
        let id = new PropertyValidator('_id').isOfType('string').isRequired(idRequired).hasLengthLimits(24, 24);
        let title = new PropertyValidator('title').isOfType('string');
        let content = new PropertyValidator('content').isOfType('string');
        return new Validator().addProperties([id, title, content]).validate(body);
    }

    validateComment(body, commentIdRequired, userIdRequired, postIdRequired) {
        let id = new PropertyValidator('_id').isOfType('string').isRequired(commentIdRequired).hasLengthLimits(24, 24);
        let content = new PropertyValidator('content').isOfType('string');
        let userId = new PropertyValidator('userId').isOfType('string').isRequired(userIdRequired);
        let postId = new PropertyValidator('postId').isOfType('string').isRequired(postIdRequired);
        return new Validator().addProperties([id, content, userId, postId]).validate(body);
    }

    validateCommentsArray(body) {
        if (!Array.isArray(body)) {
            return new BadRequestResponse({
                'message': `Invalid request body! Expected array of comments!`
            });
        }
        for (let i = 0; i < body.length; ++i) {
            let commentValidation = this.validateComment(body[i], false, true, true);
            if (commentValidation != true) {
                return commentValidation;
            }
        }
        return true;
    }
}

module.exports = PostsController;
