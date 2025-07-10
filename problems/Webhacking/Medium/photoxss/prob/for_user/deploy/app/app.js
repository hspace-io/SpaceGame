const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs');
const mysql = require('mysql2');
const session = require('express-session');
const bodyParser = require('body-parser');
const cookieParser = require('cookie-parser');
const sanitizeFilename = require('sanitize-filename');
const puppeteer = require('puppeteer');
const delay = (milliseconds) => new Promise((resolve) => setTimeout(resolve, milliseconds));

const app = express();
const port = 3000;
app.use(cookieParser());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

const generateRandomString = (length) => {
  let randomString = '';
  const characters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
  for (let i = 0; i < length; i++) {
    const randomIndex = Math.floor(Math.random() * characters.length);
    randomString += characters[randomIndex];
  }
  return randomString;
};


const SESSION_SECRET = generateRandomString(32);

const FLAG = "hspace{fake_flag}";
const cookies = [
  { name: 'flag', value: FLAG, domain: '127.0.0.1' },
]

app.use(session({
  secret: SESSION_SECRET,
  resave: false,
  saveUninitialized: true,
  cookie: { secure: false } 
}));

app.set('trust proxy', () => true);

const conn = mysql.createPool({
  host: 'db',
  user: 'hspace',
  password: 'hspace_pw',
  database: 'photoxss',
});

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.set('trust proxy', true);
app.use('/uploads', express.static(path.join(__dirname, 'uploads')));

const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    const randomString = generateRandomString(16);
    const uploadPath = path.join(__dirname, 'uploads', randomString);

    fs.mkdirSync(uploadPath, { recursive: true });
    cb(null, uploadPath);
  },
  filename: (req, file, cb) => {
    const sanitizedFileName = sanitizeFilename(file.originalname);
    cb(null, sanitizedFileName);
  },
});

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

// Authentication middleware
const authenticate = (req, res, next) => {
  if (!req.session || !req.session.user) {
    res.send('<script>alert("login plzzzz");history.back()</script>');
  } else {
    next();
  }
};

// Index route
app.get('/', (req, res) => {
  res.render('index');
});

// File upload route
app.route('/upload')
  .get(authenticate, (req, res) => {
    res.render('upload');
  })
  .post(authenticate, upload.single('file'), (req, res) => {
    if (!req.file) {
      return res.status(400).send('File not found');
    }

    const randomString = path.basename(path.dirname(req.file.path));
    const uploadedPath = `/uploads/${randomString}/${req.file.filename}`;
    
    res.send(`File uploaded successfully. Image link: <a href="${uploadedPath}" target="_blank">${uploadedPath}</a>`);
  });

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

app.get("/report", (req, res) => {
  res.render('report');
})
  

app.post("/report", (req, res) => {
  (async () => {
    const browser = await puppeteer.launch({
      executablePath: '/usr/bin/google-chrome',
      args: ["--no-sandbox"]
    });
    const page = await browser.newPage();
    await page.setCookie(...cookies);

    await page.goto(req.body.url);
    await delay(500);

    await browser.close();
  })();
  res.end('Reported!');
})

app.listen(port, () => {
  console.log(`Server is running on port ${port}...`);
});
