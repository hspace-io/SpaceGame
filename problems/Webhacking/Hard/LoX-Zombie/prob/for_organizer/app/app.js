const express = require('express');
const puppeteer = require('puppeteer-core');
const app = express();
app.use(express.urlencoded({ extended: false }));

const FLAG = 'hspace{a_leaKeD_zomBi3}';
const port = 80;
let HTML = '';

app.use((req, res, next) => {
    res.set('Cache-Control', 'no-store');
    next();
})

const isLocal = () => (req, res, next) => (req.connection.remoteAddress === '::ffff:127.0.0.1'|| req.connection.remoteAddress === '::1') 
    ? next() 
    : res.status(403).send('Only LOCAL is allowed');

app.get('/', (req, res) => {
    res.send('hello!!');
});

//isLocal()
app.get('/flag',isLocal(),(req, res) => {
    if (!('flag' in req.query))
        return res.status(200).send('give me flag');

    let flag = req.query.flag;

    if (typeof flag !== 'string')
        return res.status(200).send('give me string type');

    for (let i = 0; i < flag.length; i++) {
        if (flag.charCodeAt(i) < 32 || flag.charCodeAt(i) > 127) {
            return res.status(200).send('plz ascii');
        }
    }

    if (FLAG.startsWith(flag)) {
        HTML = `<button type='submit' id='Zombie'>good try!!</button>`;
        res.send(HTML); // HTML 반환
        HTML = '';
    } else {
        res.status(200).send('Flag not matched.');
    }
});

app.get('/report', (req, res) => {
    res.send(`
    <form method="post" action="/report">
    <input type="text" id="url" name="url"><br><br>
    <input type="submit" value="submit">
    </form>
`);
});

const visit = async (url) => {
    let browser;
    try {
        browser = await puppeteer.launch({
            headless: 'new',
            executablePath: '/usr/bin/google-chrome',
            args: [
                '--incognito',
                '--no-sandbox',
                '--disable-gpu',
                '--disable-dev-shm-usage',
                '--ignore-certificate-errors',
                '--js-flags=--noexpose_wasm,--jitless'
                ]            
        });

        let page = await browser.newPage();
        await page.goto(url, { timeout: 3000, waitUntil: 'domcontentloaded' })
        await page.waitForTimeout(60000);

        await browser.close();
        browser = null;
    } catch (err) {
        console.log(err);
    } finally {
        if (browser) await browser.close();
    }
};

app.post('/report', async (req, res) => {
    const url = req.body.url;

    try {
        if (!url.startsWith('http://') && !url.startsWith('https://')) {
            return res.send('invalid protocol');
        }

        console.log('received url: ', url);

        await visit(url);

        res.send('done');
    } catch (error) {
        console.error('Error:', error.message);
        res.send('error');
    }
});

app.listen(port, () => {
    console.log(`server is running on port ${port}...`);
});
