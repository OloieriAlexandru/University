const MongoClient = require('mongodb').MongoClient;
const Olo = require('./Olo.js').Olo;
const config = require('./config.js');
const server = require('./server');

async function runOlo() {

    const uri = `mongodb+srv://${config.MONGO_USERNAME}:${config.MONGO_PASSWORD}@olo.1yoj5.mongodb.net/${config.MONGO_DB}?retryWrites=true&w=majority`;
    const mongoClient = new MongoClient(uri, {
        useNewUrlParser: true,
        useUnifiedTopology: true
    });
    await mongoClient.connect();

    var commentsRepository = new server.CommentsRepository(mongoClient);

    var usersRepository = new server.UsersRepository(mongoClient);
    var usersService = new server.UsersService(usersRepository);
    var usersController = new server.UsersController(usersService);

    var postsRepository = new server.PostsRepository(mongoClient);
    var postsService = new server.PostsService(postsRepository, commentsRepository);
    var postsController = new server.PostsController(postsService, usersRepository);

    var guard = new server.Guard(postsRepository, commentsRepository, usersRepository);

    var olo = new Olo();

    olo.get('/api/posts', postsController.getAll);
    olo.get('/api/posts/{id:alphanum}', postsController.getById);
    olo.post('/api/posts', postsController.create,
        [guard.isLoggedIn]);
    olo.post('/api/posts/{id:alphanum}', postsController.createIdSpecified);
    olo.put('/api/posts/{id:alphanum}', postsController.update,
        [guard.isLoggedIn, guard.validPostIdInPath, guard.userIsPostOwner]);
    olo.patch('/api/posts/{id:alphanum}', postsController.patch);
    olo.delete('/api/posts/{id:alphanum}', postsController.delete,
        [guard.isLoggedIn, guard.validPostIdInPath, guard.userIsPostOwner]);

    olo.get('/api/posts/{id:alphanum}/comments',
        postsController.getAllComments, [guard.validPostIdInPath]);
    olo.get('/api/posts/{id:alphanum}/comments/{commentId:alphanum}',
        postsController.getCommentById, [guard.validPostIdInPath]);
    olo.post('/api/posts/{id:alphanum}/comments',
        postsController.createComment, [guard.isLoggedIn, guard.validPostIdInPath]);
    olo.put('/api/posts/{id:alphanum}/comments/{commentId:alphanum}',
        postsController.updateComment, [guard.isLoggedIn, guard.validPostIdInPath, guard.validCommentIdInPath]);
    olo.put('/api/posts/{id:alphanum}/comments',
        postsController.updateComments, [guard.isLoggedIn, guard.validPostIdInPath, guard.userIsPostOwner]);
    olo.delete('/api/posts/{id:alphanum}/comments',
        postsController.deleteComments, [guard.isLoggedIn, guard.validPostIdInPath, guard.userIsPostOwner])
    olo.delete('/api/posts/{id:alphanum}/comments/{commentId:alphanum}',
        postsController.deleteComment, [guard.isLoggedIn, guard.validPostIdInPath,
            guard.validCommentIdInPath, guard.userIsCommentOwner
        ]);

    olo.post('/api/users', usersController.create);
    olo.post('/api/users/login-token', usersController.login);
    olo.delete('/api/users/{id:alphanum}',
        usersController.delete, [guard.isLoggedIn, guard.validUserIdInPath]);

    olo.startServer(config.PORT, config.HOSTNAME, () => {
        console.log(`Listening at ${config.HOSTNAME}:${config.PORT}`);
    });
}

runOlo();
