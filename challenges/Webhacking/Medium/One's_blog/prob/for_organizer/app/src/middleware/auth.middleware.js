const jwt = require('jsonwebtoken');
require('dotenv').config();

exports.checkAuth = (req, res, next) => {
    try {
        const token = req.cookies.token;

        if (token) {
            const decoded = jwt.verify(token, process.env.JWT_SECRET);
            req.user = decoded;
        }

        next();
    } catch (error) {
        req.user = null;
        next();
    }
};

exports.requireAuth = (req, res, next) => {
    if (!req.user) {
        return res.status(401).render('error', { 
            message: '로그인이 필요한 서비스입니다.',
            user: null
        });
    }
    next();
};

exports.verifyToken = (req, res, next) => {
    const token = req.headers['authorization']?.split(' ')[1];

    if (!token) {
        return res.status(403).json({ message: '토큰이 제공되지 않았습니다.' });
    }

    try {
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        req.user = decoded;
        next();
    } catch (err) {
        return res.status(401).json({ message: '유효하지 않은 토큰입니다.' });
    }
}; 