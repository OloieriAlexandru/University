const BadRequestResponse = require('../Olo.js').BadRequestResponse;
const UnauthorizedResponse = require('../Olo.js').UnauthorizedResponse;
const ForbiddenResponse = require('../Olo.js').ForbiddenResponse;
const NotFoundResponse = require('../Olo.js').NotFoundResponse;

const JwtUtil = require('./jwt-util');

class Guard {
    constructor(postsRepository, commentsRepository, usersRepository) {
        this.postsRepository = postsRepository;
        this.commentsRepository = commentsRepository;
        this.usersRepository = usersRepository;
    }

    validPostIdInPath = async (req, res) => {
        let postId = req.olo.urlVars['id'];
        let post = await this.postsRepository.getById(postId);
        if (!post) {
            return new NotFoundResponse({
                message: `Invalid request route! Invalid post id (${postId})`
            });
        }
        return true;
    }

    userIsPostOwner = async (req, res) => {
        let postId = req.olo.urlVars['id'];
        let post = await this.postsRepository.getById(postId);
        if (post.userId != req.olo.userId) {
            return new ForbiddenResponse();
        }
        return true;
    }

    userIsCommentOwner = async (req, res) => {
        let postId = req.olo.urlVars['id'];
        let commentId = req.olo.urlVars['commentId'];
        let comment = await this.commentsRepository.getByIdOnPost(postId, commentId);
        if (!comment) {
            return new BadRequestResponse({
                message: `Invalid request route! Invalid comment id (${commentId})`
            });
        }
        if (!comment.userId || comment.userId != req.olo.userId) {
            return new UnauthorizedResponse({
                message: 'You are not allowed to do that! You\'re not the writer of the comment!'
            })
        }
        return true;
    }

    validCommentIdInPath = async (req, res) => {
        let postId = req.olo.urlVars['id'];
        let commentId = req.olo.urlVars['commentId'];
        let comment = await this.commentsRepository.getByIdOnPost(postId, commentId);
        if (!comment) {
            return new NotFoundResponse({
                message: `Invalid request route! Invalid comment id (${commentId})`
            });
        }
        return true;
    }

    validUserIdInPath = async (req, res) => {
        let userId = req.olo.urlVars['id'];
        return await this.validateUserById(req, res, userId);
    }

    isLoggedIn = async (req, res) => {
        let result = JwtUtil.guardIsLoggedIn(req, res);
        result &= await this.validateUserById(req, res, req.olo.userId);
        if (result != true) {
            return new UnauthorizedResponse({
                message: 'Invalid or missing JWT token!'
            });
        }
        return true;
    }

    validateUserById = async (req, res, userId) => {
        let user = await this.usersRepository.getById(userId);
        if (!user) {
            return new BadRequestResponse({
                message: `Invalid request route! Invalid user id ({${userId}})`
            });
        }
        if (userId != req.olo.userId) {
            return new ForbiddenResponse();
        }
        return true;
    }
}

module.exports = Guard;
