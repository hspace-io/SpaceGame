const express = require('express');
const router = express.Router();
const authController = require('../controllers/auth.controller');

router.get('/login', (req, res) => {
    res.render('auth/login', { 
        title: '로그인',
        user: req.user
    });
});

router.get('/register', (req, res) => {
    res.render('auth/register', { 
        title: '회원가입',
        user: req.user
    });
});

router.get('/logout', (req, res) => {
    res.clearCookie('token');
    res.redirect('/');
});

router.post('/register', authController.register);
router.post('/login', authController.login);

module.exports = router;
