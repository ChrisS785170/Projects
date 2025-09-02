Vue.component('navigation-bar', {
    data: function () {
      return {
        isLoggedIn: false,
        username: '',
        isAdmin: false
      };
    },
    mounted: function() {
      const xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = () => {
        if (xhttp.readyState === 4 && xhttp.status === 200) {
          const data = JSON.parse(xhttp.responseText);
          this.username = data.username;
          this.isAdmin = data.is_admin;
          this.isLoggedIn = true;
        } else if (xhttp.readyState === 4) {
          console.log('Session user not found');
        }
      };
      xhttp.onerror = () => {
        console.log('An error occurred during the transaction');
      };
      xhttp.open("GET", "/sessionuser");
      xhttp.send();
    },
    template: `
      <nav>
        <div class="nav-logo">
          <a href="/welcome.html">Dodgyclubs</a>
        </div>
        <div class="nav-links">
          <ul>
            <li><a href="/findaclub.html">Find a Club</a></li>
            <li v-if="isLoggedIn"><a href="/myclubs.html">My Clubs</a></li>
            <li><a href="/about.html">About</a></li>
            <li v-if="isAdmin"><a href="/admin.html">Admin</a></li>
          </ul>
        </div>
        <div class="nav-user">
          <a class="nav-log-in" v-if="!isLoggedIn" href="/login.html">Login</a>
          <a href="/profile.html">
            <div class="user-dropdown" v-if="isLoggedIn">
              <span v-if="isLoggedIn">{{ username }}</span>
              <img src="images/default_avatar.png" alt="Avatar" />
              <div class="user-dropdown-content">
                <a href="/profile.html">Profile</a>
                <a href="#" v-on:click="logout();">Logout</a>
              </div>
            </div>
          </a>
        </div>
      </nav>
    `,
    methods: {
      logout() {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (xhttp.readyState === 4) {
            if (xhttp.status === 200) {
              alert('See you soon!');
              window.location.reload();
            } else if (xhttp.status === 403) {
              alert('You are not logged in');
            }
          }
        };

        xhttp.open('POST', '/logout');
        xhttp.send();
    }
  }
});
