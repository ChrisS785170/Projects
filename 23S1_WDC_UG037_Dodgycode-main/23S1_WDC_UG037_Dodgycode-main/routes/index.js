var express = require('express');
var router = express.Router();

const bcrypt = require('bcrypt');
const saltRounds = 10;

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

// for /admin.html user list
router.get('/users', function(req, res){
  const query = 'SELECT * FROM Users';
  req.pool.query(query, function(error, results){
    if (error) {
      console.error(error);
      res.sendStatus(500);
    } else {
      res.json(results);
    }
  });
});

// for /admin.html and /findaclub.html club list
router.get('/clubs', function(req, res) {
  const query = 'SELECT * FROM Clubs';
  req.pool.query(query, function(error, results){
    if (error) {
      console.error(error);
      res.status(500).send('Internal Server Error');
    } else {
      res.status(200).json(results);
    }
  });
});

router.post('/users/:userId/makeadmin', function(req, res){
  if (!req.session.userId || !req.session.isAdmin) {
    return res.status(403).json({ error: 'You must be an admin to do that' });
  }

  const userId = req.params.userId;

  const query = 'UPDATE Users SET is_admin = true WHERE user_id = ?';

  req.pool.query(query, [userId], function(error, results){
    if (error) {
      console.error(error);
      res.sendStatus(500);
    } else {
      res.json({ success: true });
    }
  });
});

// for /profile.html info
router.get('/user', function(req, res){
  if (req.session.userId) {
    const query = 'SELECT * FROM Users WHERE user_id = ?';
    req.pool.query(query, [req.session.userId], function(error, results){
      if (error) {
        console.error(error);
        res.sendStatus(500);
      } else if (results.length > 0) {
        const user = results[0];
        res.json({ id: user.user_id, ...user });
      } else {
        res.sendStatus(404);
      }
    });
  } else {
    res.sendStatus(401);
  }
});

// for profile.html club list
router.get('/users/profileclubs', function(req, res) {
  const userId = req.session.userId;

  const sql = `
    SELECT Clubs.name
    FROM Clubs
    INNER JOIN Memberships ON Clubs.club_id = Memberships.club_id
    WHERE Memberships.user_id = ?
  `;

  req.pool.query(sql, [userId], function(error, results){
    if (error) {
      console.log(error);
      res.status(500).send('An error occurred while querying the database');
      return;
    }

    res.json(results);
  });
});

// for /club.html info
router.get('/clubs/:club_id', function(req, res, next) {
  const clubId = req.params.club_id;

  const query = 'SELECT * FROM Clubs WHERE club_id = ?';
  req.pool.query(query, [clubId], function(error, results){
    if (error) {
      console.error(error);
      res.sendStatus(500);
    } else if (results.length === 0) {
      res.sendStatus(404);
    } else {
      const club = results[0];
      res.json(club);
    }
  });
});

// for /club.html RSVP list
router.get('/clubs/:clubId/rsvps', function(req, res) {
  const sql = `SELECT Users.username, Users.firstname, Users.lastname, Events.name as eventName
               FROM Rsvp
               INNER JOIN Users ON Rsvp.user_id = Users.user_id
               INNER JOIN Events ON Rsvp.event_id = Events.event_id
               WHERE Events.club_id = ?`;

  req.pool.query(sql, [req.params.clubId], function(err, results) {
      if (err) {
          res.status(500).send(err);
      } else {
          res.json(results);
      }
  });
});

// for getting /club.html updates
router.get('/clubs/:clubId/updates', function(req, res){
  const sql = 'SELECT * FROM Updates WHERE club_id = ?';
  req.pool.query(sql, [req.params.clubId], function(err, result){
      if (err) {
          res.status(500).send(err);
      } else {
          res.send(result);
      }
  });
});

// for getting /club.html evvents
router.get('/clubs/:clubId/events', function(req, res){
  const sql = 'SELECT * FROM Events WHERE club_id = ?';
  req.pool.query(sql, [req.params.clubId], function(err, result){
      if (err) {
          res.status(500).send(err);
      } else {
          res.send(result);
      }
  });
});

// for list of club members on /club.html
router.get('/clubusers/:clubId', function(req, res) {
  const clubId = req.params.clubId;

  if (!clubId) {
    return res.status(400).json({ error: 'Missing club id in URL params.' });
  }

  const query = 'SELECT Users.user_id, Users.username, Users.firstname, Users.lastname FROM Users JOIN Memberships ON Users.user_id = Memberships.user_id WHERE Memberships.club_id = ?';

  req.pool.query(query, [clubId], function(error, results) {
    if (error) {
      console.log(error);
      res.status(500).json({ error: 'An error occurred while fetching users.' });
      return;
    }

    if (results.length === 0) {
      res.status(404).json({ error: 'Users not found.' });
      return;
    }

    const users = results.map(user => {
      return {
        user_id: user.user_id,
        username: user.username,
        firstname: user.firstname,
        lastname: user.lastname
      };
    });

    res.json(users);
  });
});

// for posting updates
router.post('/clubs/:clubId/updates', function(req, res) {
  if (!req.session.isAdmin) {
    res.status(403).send('You must be an admin to do that.');
    return;
  }
  const sql = 'INSERT INTO Updates (content, timestamp, club_id) VALUES (?, NOW(), ?)';
  req.pool.query(sql, [req.body.content, req.params.clubId], function(err, result) {
    if (err) {
      console.error(err);
      res.status(500).send('An error occurred');
    } else {
      res.status(201).send('Update created successfully');
    }
  });
});

// for posting events
router.post('/clubs/:clubId/events', function(req, res) {
  if (!req.session.isAdmin) {
    res.status(403).send('You must be an admin to do that.');
    return;
  }
  const sql = 'INSERT INTO Events (name, description, event_timestamp, club_id) VALUES (?, ?, ?, ?)';
  req.pool.query(sql, [req.body.name, req.body.description, req.body.event_timestamp,
    req.params.clubId], function(err, result) {
    if (err) {
      console.error(err);
      res.status(500).send('An error occurred');
    } else {
      res.status(201).send('Event created successfully');
    }
  });
});

router.get('/users/myupdates', function(req, res) {
  const userId = req.session.userId;
  const sql = `
    SELECT Updates.*
    FROM Updates
    INNER JOIN Memberships ON Updates.club_id = Memberships.club_id
    WHERE Memberships.user_id = ?
  `;
  req.pool.query(sql, [userId], function(error, results){
    if (error) {
      console.log(error);
      res.status(500).send('An error occurred while querying the database');
      return;
    }
    res.json(results);
  });
});

router.get('/users/myevents', function(req, res) {
  const userId = req.session.userId;
  const sql = `
    SELECT Events.*
    FROM Events
    INNER JOIN Memberships ON Events.club_id = Memberships.club_id
    WHERE Memberships.user_id = ?
  `;
  req.pool.query(sql, [userId], function(error, results){
    if (error) {
      console.log(error);
      res.status(500).send('An error occurred while querying the database');
      return;
    }
    res.json(results);
  });
});

router.post('/signup', function(req, res, next) {
  req.pool.getConnection(function(cerr, connection) {
    if (cerr) {
      console.log(cerr);
      res.sendStatus(500);
      return;
    }

    let checkQuery = "SELECT * FROM Users WHERE username = ? OR email = ?";

    connection.query(checkQuery, [req.body.username, req.body.email], function(chkErr, chkResult) {
      if (chkErr) {
        console.log(chkErr);
        res.sendStatus(500);
        return;
      }

      if (chkResult.length > 0) {
        // user with provided username or email already exists
        res.status(409).send('Username or email already taken');
        connection.release();
        return;
      }

      let insertQuery = `INSERT INTO Users (
                          username,
                          email,
                          firstname,
                          lastname,
                          password
                        ) VALUES (?,?,?,?,?)`;

      if (req.body.password !== req.body.confirm_password) {
        res.sendStatus(401);
        return;
      }

      bcrypt.hash(req.body.password, saltRounds, function(err, hash) {
        if (err) {
          console.log(err);
          res.sendStatus(500);
          return;
        }

        connection.query(insertQuery, [req.body.username, req.body.email, req.body.firstname,
          req.body.lastname, hash], function(insErr, insResult) {
          connection.release();
          if (insErr) {
            res.sendStatus(401);
            return;
          }
          res.end();
        });
      });
    });
  });
});

router.post('/login', function(req, res, next) {
  req.pool.getConnection(function(cerr, connection) {
    if (cerr) {
      res.sendStatus(500);
      return;
    }

    let query = "SELECT user_id, username, email, firstname, lastname, profile_picture, is_admin, password FROM Users WHERE username = ?";

    connection.query(query, [req.body.username], function(qerr, rows, fields) {
      connection.release();
      if (qerr) {
        res.sendStatus(500);
        return;
      }
      console.log(JSON.stringify(rows));
      if (rows.length > 0) {
        // user exists
        let [user] = rows;

        bcrypt.compare(req.body.password, user.password, function(err, result) {
          if (err) {
            res.sendStatus(500);
            return;
          }

          if (result) {
            // passwords match
            req.session.userId = user.user_id;
            req.session.isAdmin = user.is_admin;
            res.json(user);
          } else {
            // passwords don't match
            res.sendStatus(401);
          }
        });
      } else {
        // user doesn't exist
        res.sendStatus(401);
      }
    });
  });
});

router.post('/logout', function(req, res, next) {
  req.session.destroy(function(err) {
    if (err) {
        res.sendStatus(500);
    } else {
        res.sendStatus(200);
    }
  });
});

router.post('/createclub', function(req, res, next) {
  if (!req.session.isAdmin) {
    res.status(403).send('You must be an admin to do that.');
    return;
  }

  req.pool.getConnection(function(cerr, connection) {
      if (cerr) {
          console.log(cerr);
          res.sendStatus(500);
          return;
      }

      let query = `INSERT INTO Clubs (name, description) VALUES (?, ?)`;

      connection.query(query, [req.body.name, req.body.description], function(err, result) {
          connection.release();
          if (err) {
              console.log(err);
              res.sendStatus(500);
              return;
          }
          res.sendStatus(200);
      });
  });
});

router.post('/joinclub', function(req, res) {
  if (req.session.userId) {
    const { clubId } = req.body;

    const checkMembershipQuery = 'SELECT * FROM Memberships WHERE club_id = ? AND user_id = ?';
    req.pool.query(checkMembershipQuery, [clubId, req.session.userId], function(error, results) {
      if (error) {
        res.status(500).send('Internal server error');
      } else if (results.length > 0) {
        res.status(400).send('Already a member of the club');
      } else {
        const addMembershipQuery = 'INSERT INTO Memberships (club_id, user_id) VALUES (?, ?)';
        req.pool.query(addMembershipQuery, [clubId, req.session.userId], function(cerror, cresults) {
          if (cerror) {
            res.status(500).json({ status: 'error', message: 'Internal Server Error' });
          } else {
            res.status(200).json({ status: 'success', message: 'Joined the club successfully' });
          }
        });
      }
    });
  } else {
    res.status(401).json({ status: 'error', message: 'Not logged in' });
  }
});

router.post('/leaveclub', function(req, res) {
  if (req.session.userId) {
    const { clubId } = req.body;

    const checkMembershipQuery = 'SELECT * FROM Memberships WHERE club_id = ? AND user_id = ?';
    req.pool.query(checkMembershipQuery, [clubId, req.session.userId], function(error, results) {
      if (error) {
        res.status(500).send('Internal server error');
      } else if (results.length > 0) {
        const removeMembershipQuery = 'DELETE FROM Memberships WHERE club_id = ? AND user_id = ?';
        req.pool.query(removeMembershipQuery, [clubId, req.session.userId], function(rerror, rresults) {
          if (rerror) {
            res.status(500).json({ status: 'error', message: 'Internal Server Error' });
          } else {
            res.status(200).json({ status: 'success', message: 'Left the club successfully' });
          }
        });
      } else {
        res.status(400).send('Not a member of the club');
      }
    });
  } else {
    res.status(401).json({ status: 'error', message: 'Not logged in' });
  }
});

router.get('/isMember', function(req, res) {
  if (req.session.userId) {
    const clubId = req.query.clubId;

    const checkMembershipQuery = 'SELECT * FROM Memberships WHERE club_id = ? AND user_id = ?';
    req.pool.query(checkMembershipQuery, [clubId, req.session.userId], function(error, results) {
      if (error) {
        res.status(500).send('Internal server error');
      } else {
        if (results.length > 0) {
          res.status(200).json({ userIsMember: true });
        } else {
          res.status(200).json({ userIsMember: false });
        }
      }
    });
  } else {
    res.status(401).json({ status: 'error', message: 'Not logged in' });
  }
});

router.post('/rsvp', function(req, res) {
  if (!req.session.userId) {
    res.status(401).json({ message: 'Unauthorized' });
    return;
  }

  const { eventId } = req.body;
  const userId = req.session.userId;

  // first, try to delete an existing RSVP
  const deleteSql = `DELETE FROM Rsvp WHERE user_id = ? AND event_id = ?`;

  req.pool.query(deleteSql, [userId, eventId], function(error, result) {
    if (error) {
      console.error('Error deleting RSVP:', error);
      res.status(500).json({ message: 'Failed to update RSVP' });
      return;
    }

    if (result.affectedRows === 0) {
      const insertSql = `INSERT INTO Rsvp (user_id, event_id) VALUES (?, ?)`;

      req.pool.query(insertSql, [userId, eventId], function(cerror, cresult) {
        if (cerror) {
          console.error('Error inserting RSVP:', cerror);
          res.status(500).json({ message: 'Failed to update RSVP' });
        } else {
          res.status(200).json({ message: 'RSVP successful', rsvpStatus: true });
        }
      });
    } else {
      res.status(200).json({ message: 'Un-RSVP successful', rsvpStatus: false });
    }
  });
});

// updating user's first name
router.post('/userfirst', function(req, res) {
  if (!req.session.userId) {
    res.status(401).json({ message: 'Not logged in' });
    return;
  }
  let userId = req.session.userId;
  let user = req.body;

    if (!user.firstname) {
        res.status(400).send('Invalid user data');
        return;
    }
    let sql = `UPDATE Users SET firstname = ? WHERE user_id = ?`;
    let data = [user.firstname, userId];

    req.pool.query(sql, data, (error, results) => {
        if (error) {
            console.error('Error updating user:', error);
            res.status(500).send('Server error');
          }else {
            res.send({ status: 'success', message: 'User updated successfully' });
        }
    });
});

// updating user's last name
router.post('/userlast', function(req, res){
  if (!req.session.userId) {
    res.status(401).json({ message: 'Not logged in' });
    return;
  }
  let userId = req.session.userId;
  let user = req.body;

  if (!user.firstname) {
      res.status(400).send('Invalid user data');
      return;
  }

  let sql = `UPDATE Users SET lastname = ? WHERE user_id = ?`;
  let data = [user.lastname, userId];

  req.pool.query(sql, data, (error, results) => {
      if (error) {
          console.error('Error updating user:', error);
          res.status(500).send('Server error');
        }else {
          res.send({ status: 'success', message: 'User updated successfully' });
      }
  });
});

// updating username
router.post('/userusername', function(req, res) {
  if (!req.session.userId) {
    res.status(401).json({ message: 'Not logged in' });
    return;
  }
  let userId = req.session.userId;
  let user = req.body;

  if (!user.username) {
      res.status(400).send('Invalid user data');
      return;
  }

  let sql = `UPDATE Users SET username = ? WHERE user_id = ?`;
  let data = [user.username, userId];

  req.pool.query(sql, data, (error, results) => {
      if (error) {
          console.error('Error updating user:', error);
          res.status(500).send('Server error');
      } else {
          res.send({ status: 'success', message: 'User updated successfully' });
      }
  });
});

module.exports = router;
