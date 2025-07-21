const Post = require("../models/post.model");

exports.getIndex = async (req, res) => {
  try {
    const posts = {};
    res.render("index", {
      posts,
      user: req.user,
      title: "홈",
    });
  } catch (error) {
    console.error("메인 페이지 로딩 중 오류:", error);
    res.status(500).render("error", {
      message: "서버 오류가 발생했습니다.",
      user: req.user,
    });
  }
};
