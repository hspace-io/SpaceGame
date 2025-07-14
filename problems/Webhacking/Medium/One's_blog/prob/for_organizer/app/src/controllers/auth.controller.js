const jwt = require("jsonwebtoken");
const User = require("../models/user.model");
require("dotenv").config();

const register = async (req, res) => {
  try {
    const { username, password } = req.body;

    if (!username || !password) {
      return res.status(400).render("auth/register", {
        error: "All fields are required.",
        user: req.user,
        title: "회원가입",
      });
    }

    if (username.length < 10 || password.length < 10) {
      return res.status(400).render("auth/register", {
        error: "사용자명 또는 비밀번호는 10자 이상이여야 합니다.",
        user: req.user,
        title: "회원가입",
      });
    }

    const existingUser = await User.findByUsername(username);
    if (existingUser) {
      return res.status(400).render("auth/register", {
        error: "Duplicate user",
        user: req.user,
        title: "회원가입",
      });
    }

    await User.create(username, password);
    res.redirect("/auth/login");
  } catch (error) {
    console.error("회원가입 오류:", error);
    res
      .status(500)
      .render("error", { message: "Server error occurred.", user: req.user });
  }
};

const login = async (req, res) => {
  try {
    const { username, password } = req.body;

    if (!username || !password) {
      return res.status(400).render("auth/login", {
        error: "All fields are required.",
        user: req.user,
        title: "로그인",
      });
    }

    if (username.length < 10 || password.length < 10) {
      return res.status(400).render("auth/login", {
        error: "사용자명 또는 비밀번호는 10자 이상이여야 합니다.",
        user: req.user,
        title: "로그인",
      });
    }

    const user = await User.findByUsername(username);
    if (!user) {
      return res.status(401).render("auth/login", {
        error: "사용자명 또는 비밀번호가 올바르지 않습니다.",
        user: req.user,
        title: "로그인",
      });
    }

    const isValidPassword = await User.verifyPassword(password, user.password);
    if (!isValidPassword) {
      return res.status(401).render("auth/login", {
        error: "사용자명 또는 비밀번호가 올바르지 않습니다.",
        user: req.user,
        title: "로그인",
      });
    }

    const token = jwt.sign(
      { id: user.id, username: user.username },
      process.env.JWT_SECRET,
      { expiresIn: "1h" }
    );

    res.cookie("token", token, { httpOnly: true });
    res.redirect("/posts");
  } catch (error) {
    console.error("로그인 오류:", error);
    res
      .status(500)
      .render("error", {
        message: "서버 오류가 발생했습니다.",
        user: req.user,
      });
  }
};

module.exports = {
  register,
  login,
};
