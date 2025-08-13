var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var session = require('express-session');
var mysql = require('mysql');

var dbConnectionPool = mysql.createPool({
  host: 'localhost',
  user: 'root',
  password: 'password',
  database: 'Dodgyclubs'
});


var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use(function(req,res,next){
  req.pool = dbConnectionPool;
  next();
});

app.use(session({
  resave: false,
  saveUninitialized: true,
  secret: 'a secret'
}));

// to display logged-in user in nav bar
app.get('/sessionuser', function(req, res) {
  if (req.session.userId) {
    const query = 'SELECT username, is_admin FROM Users WHERE user_id = ?';
    req.pool.query(query, [req.session.userId], function(error, results){
      if (error) {
        console.error(error);
        res.sendStatus(500);
      } else if (results.length > 0) {
        const user = results[0];
        res.json(user);
      } else {
        res.sendStatus(404);
      }
    });
  } else {
    res.sendStatus(401);
  }
});

app.use('/', indexRouter);
app.use('/users', usersRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
