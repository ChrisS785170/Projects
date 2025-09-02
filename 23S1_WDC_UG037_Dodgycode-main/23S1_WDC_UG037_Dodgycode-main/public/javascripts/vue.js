const vueinst = new Vue({
  el: '#app',
  data: {
    clubSearchQuery: '',
    username: '',
    email: '',
    password: '',
    confirm_password: '',
    firstname: '',
    lastname: '',
    users: [],
    profileClubs: [],
    myClubsUpdates: {},
    myClubsEvents: {},
    clubs: [],
    rsvps: [],
    newClub: {
      name: '',
      description: ''
    },
    user: {},
    selectedClub: {},
    clubMembers: {},
    userIsMember: '',
    newUpdate: {
      content: ''
    },
    newEvent: {
      name: '',
      description: '',
      event_timestamp: ''
    },
    editFirstname: false,
    editLastname: false,
    editEmail: false,
    editUsername: false
  },
  computed: {
    filteredClubs() {
      if (this.clubSearchQuery) {
        return this.clubs.filter((club) => {
          return club.name.toLowerCase().includes(this.clubSearchQuery.toLowerCase());
        });
      } else {
        return this.clubs;
      }
    }
  },
  methods: {
    login() {
      const loginData = {
        username: this.username,
        password: this.password
      };

      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            alert('Logged in successfully');
            window.location.reload();
          } else if (this.status === 401) {
            alert('Login failed, please try again');
          }
        }
      };

      xhttp.open('POST', '/login');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(loginData));
    },
    signup() {
      const signupData = {
        username: this.username,
        email: this.email,
        firstname: this.firstname,
        lastname: this.lastname,
        password: this.password,
        confirm_password: this.confirm_password
      };

      if (signupData.password !== signupData.confirm_password) {
        alert('Your passwords do not match');
        return;
      }

      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            alert('Registration successful, you can now login');
            window.location.reload();
          } else if (this.status === 401) {
            alert('Registration failed, please try again');
          }
        }
      };

      xhttp.open('POST', '/signup');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(signupData));
    },
    getUsers() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
          vueinst.users = data;
        }
      };

      xhttp.open('GET', '/users');
      xhttp.send();
    },
    getClubMembers(clubId) {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (xhttp.readyState === 4) {
          if (xhttp.status === 200) {
            vueinst.clubMembers = JSON.parse(xhttp.responseText);
            console.log(vueinst.clubMembers);
          } else {
            console.log('Error:', xhttp.status);
          }
        }
      };
      xhttp.open('GET', `/clubusers/${clubId}`);
      xhttp.send();
    },
    getClubs() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
          vueinst.clubs = data;
        }
      };

      xhttp.open('GET', '/clubs');
      xhttp.send();
    },
    createClub() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
          if (data.status === 'success') {
            vueinst.getClubs();
          }
        }
      };

      xhttp.open('POST', '/createclub');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(this.newClub));
    },
    createEvent() {
      const eventData = {
        name: this.newEvent.name,
        description: this.newEvent.description,
        event_timestamp: this.newEvent.event_timestamp,
        club_id: this.selectedClub.club_id
      };
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 201) {
            alert('Event created successfully');
            window.location.reload();
          } else {
            alert('An error occurred while creating the event');
          }
        }
      };
      xhttp.open('POST', `/clubs/${eventData.club_id}/events`);
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(eventData));
    },
    createUpdate() {
      const updateData = {
        content: this.newUpdate.content,
        club_id: this.selectedClub.club_id
      };

      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 201) {
            alert('Update created successfully');
            window.location.reload();
          } else {
            alert('An error occurred while creating the update');
          }
        }
      };

      xhttp.open('POST', `/clubs/${updateData.club_id}/updates`); // Use updateData.club_id
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(updateData));
    },
    getUserData() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
          vueinst.user = data;
        }
      };

      xhttp.open('GET', '/user');
      xhttp.send();
    },
    getClubData(clubId) {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
          vueinst.selectedClub = data;
          vueinst.userIsMember = data.isMember;
        }
      };

      xhttp.open('GET', `/clubs/${clubId}`);
      xhttp.send();

      const updatesXhttp = new XMLHttpRequest();
      updatesXhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const updates = JSON.parse(this.responseText);
          vueinst.$set(vueinst.selectedClub, 'updates', updates);
        }
      };

      updatesXhttp.open('GET', `/clubs/${clubId}/updates`);
      updatesXhttp.send();

      const eventsXhttp = new XMLHttpRequest();
      eventsXhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const events = JSON.parse(this.responseText);
          vueinst.$set(vueinst.selectedClub, 'events', events);
        }
      };

      eventsXhttp.open('GET', `/clubs/${clubId}/events`);
      eventsXhttp.send();
    },
    fetchProfileClubs() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
      if (this.readyState === 4 && this.status === 200) {
         vueinst.profileClubs = JSON.parse(this.responseText);
      }
    };
    xhttp.open("GET", '/users/profileclubs', true);
    xhttp.send();
    },
    fetchMyUpdates: function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
           vueinst.myClubsUpdates = JSON.parse(this.responseText);
        }
      };
      xhttp.open("GET", '/users/myupdates', true);
      xhttp.send();
    },
    fetchMyEvents: function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
           vueinst.myClubsEvents = JSON.parse(this.responseText);
        }
      };
      xhttp.open("GET", `/users/myevents`, true);
      xhttp.send();
    },
    makeAdmin(userId) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
          if (this.readyState === 4 && this.status === 200) {
              for (var i = 0; i < vueinst.users.length; i++) {
                  if (vueinst.users[i].user_id === userId) {
                      vueinst.users[i].isAdmin = true;
                      break;
                  }
              }
          } else if (this.readyState === 4) {
              console.error("There was an error making this user an admin.");
          }
      };
      xhttp.open("POST", `/users/${userId}/makeadmin`, true);
      xhttp.setRequestHeader("Content-Type", "application/json");
      xhttp.send();
    },
    joinClub() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            vueinst.userIsMember = true;
          } else {
            const errorData = JSON.parse(this.responseText);
            console.log(errorData.message);
          }
        }
      };

      xhttp.open('POST', '/joinclub');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify({
        clubId: this.selectedClub.club_id
      }));
    },
    leaveClub() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            vueinst.userIsMember = false;
          } else {
            const errorData = JSON.parse(this.responseText);
            console.log(errorData.message);
          }
        }
      };

      xhttp.open('POST', '/leaveclub');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify({
        clubId: this.selectedClub.club_id
      }));
    },
    fetchUserIsMember() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            const responseData = JSON.parse(this.responseText);
            vueinst.userIsMember = responseData.userIsMember;
          } else {
            console.log('Failed to fetch userIsMember');
          }
        }
      };

      xhttp.open('GET', '/isMember?clubId=' + encodeURIComponent(this.selectedClub.club_id));
      xhttp.send();
    },
    rsvpToEvent(event) {
      const rsvpData = {
        eventId: event.event_id
      };

      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4) {
          if (this.status === 200) {
            const response = JSON.parse(this.responseText);
            event.rsvpStatus = response.rsvpStatus;
            alert(response.message);
          } else {
            alert('An error occurred while RSVPing');
          }
        }
      };

      xhttp.open('POST', '/rsvp');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(rsvpData));
    },
    loadRsvps(clubId){
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          vueinst.rsvps = JSON.parse(this.responseText);
        }
      };
      xhttp.open("GET", `/clubs/${clubId}/rsvps`, true);
      xhttp.send();
    },
    updateUsername() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
        }
      };

      xhttp.open('POST', '/userusername');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(this.user));
    },
    updateUserfirst() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
        }
      };

      xhttp.open('POST', '/userfirst');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(this.user));
    },
    updateUserlast() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState === 4 && this.status === 200) {
          const data = JSON.parse(this.responseText);
        }
      };

      xhttp.open('POST', '/userlast');
      xhttp.setRequestHeader('Content-Type', 'application/json');
      xhttp.send(JSON.stringify(this.user));
    }
    },
    watch: {
      selectedClub() {
        this.fetchUserIsMember();
      }
    },
  async mounted() {
    if (window.location.pathname === '/admin.html') {
      this.getUsers();
    }
    if (window.location.pathname.includes('club') || window.location.pathname === '/admin.html') {
      this.getClubs();
    }
    if (window.location.pathname === '/profile.html') {
      this.getUserData();
      this.fetchProfileClubs();
    }
    if (window.location.pathname === '/myclubs.html') {
      this.fetchMyUpdates();
      this.fetchMyEvents();
    }
    if (window.location.pathname.includes('/club.html')) {
      const urlParams = new URLSearchParams(window.location.search);
      this.fetchUserIsMember();
      const clubId = urlParams.get('id');
      if (clubId) {
        await this.getClubData(clubId);
        this.getClubMembers(clubId);
        this.loadRsvps(clubId);
      }
    }
  }
});

