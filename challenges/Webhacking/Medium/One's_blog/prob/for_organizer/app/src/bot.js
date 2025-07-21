const puppeteer = require("puppeteer-core");
const crypto = require("crypto");

const FLAG =
  "hspace{w0w_4aws0me_CSRF_aTTacK!!_W1tH_D0OOOM_Pur1fYyYy_s4n1t1ze_ByPaSsSs1ng!}";
const botVisited = {};

const visit = async (POST_ID, ID, PS) => {
  let browser;
  try {
    browser = await puppeteer.launch({
      headless: "new",
      executablePath: "/usr/bin/google-chrome",
      args: [
        "--no-sandbox",
        "--disable-dev-shm-usage",
        "--disable-gpu",
        "--incognito",
        "--js-flags=--noexpose_wasm,--jitless",
      ],
    });

    let page = await browser.newPage();

    await page.goto("http://localhost:3000/auth/login", {
      timeout: 1000,
      waitUntil: "domcontentloaded",
    });

    await page.evaluate(
      (ID, PS) => {
        document.querySelector("#username").value = ID;
        document.querySelector("#password").value = PS;
        document.querySelector("#submit").click();
      },
      ID,
      PS
    );

    await page.waitForTimeout(1000);

    console.log(`http://localhost:3000/posts/${POST_ID}`);

    await page.goto(`http://localhost:3000/posts/${POST_ID}`, {
      timeout: 2000,
      waitUntil: "domcontentloaded",
    });
    await page.waitForTimeout(2000);

    await page.close();
    await browser.close();

    browser = null;
  } catch (err) {
    console.log("bot error", err);
  } finally {
    if (browser) await browser.close();
  }
};

exports.bot = async (post_id) => {
  if (botVisited[post_id]) {
    return false;
  }

  botVisited[post_id] = true;

  // user1
  const username1 = crypto.randomBytes(16).toString("hex");
  const password1 = crypto.randomBytes(16).toString("hex");

  // user2
  const username2 = crypto.randomBytes(16).toString("hex");
  const password2 = crypto.randomBytes(16).toString("hex");

  // setting comment
  await fetch("http://localhost:3000/auth/register", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `username=${username1}&password=${password1}`,
  });

  const login_res1 = await fetch("http://localhost:3000/auth/login", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `username=${username1}&password=${password1}`,
    redirect: "manual",
  });

  const session1 = login_res1.headers.getSetCookie()[0];

  await fetch(`http://localhost:3000/posts/${post_id}/comments`, {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
      Cookie: session1,
    },
    body: `content=${username2}`,
  });

  // setting flag
  await fetch("http://localhost:3000/auth/register", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `username=${username2}&password=${password2}`,
  });

  const login_res2 = await fetch("http://localhost:3000/auth/login", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `username=${username2}&password=${password2}`,
    redirect: "manual",
  });

  const session2 = login_res2.headers.getSetCookie()[0];

  await fetch(`http://localhost:3000/posts`, {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
      Cookie: session2,
    },
    body: `title=th1s1smy2ecreTS0C0ntentD4ta&content=${FLAG}`,
  });

  // visit Page
  visit(post_id, username2, password2);
};
