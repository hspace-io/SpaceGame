const Post = require("../models/post.model");
const Comment = require("../models/comment.model");
const { bot } = require("../bot");

const getPosts = async (req, res) => {
  try {
    const posts = await Post.findAll(req.user?.id);
    res.render("posts/index", {
      posts,
      user: req.user,
      title: "게시물 목록",
    });
  } catch (error) {
    console.error("게시물 목록 조회 오류:", error);
    res.status(500).render("error", {
      message: "게시물 목록을 불러오는 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const getCreateForm = (req, res) => {
  res.render("posts/create", {
    user: req.user,
    title: "게시물 작성",
  });
};

const createPost = async (req, res) => {
  try {
    const { title, content } = req.body;

    if (!title || !content) {
      return res.status(400).render("posts/create", {
        errors: [{ msg: "제목과 내용을 모두 입력해주세요." }],
        user: req.user,
        title: "게시물 작성",
      });
    }
    const postId = await Post.create(title, content, false, req.user.id);
    res.redirect(`/posts/${postId}`);
  } catch (error) {
    console.error("게시물 작성 오류:", error);
    res.status(500).render("error", {
      message: "게시물 작성 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const getPost = async (req, res) => {
  try {
    const post = await Post.findById(req.params.id);
    if (!post) {
      return res.status(404).render("error", {
        message: "게시물을 찾을 수 없습니다.",
        user: req.user,
      });
    }

    if (!post.is_public && (!req.user || post.user_id !== req.user.id)) {
      return res.status(403).render("error", {
        message: "이 게시물에 접근할 권한이 없습니다.",
        user: req.user,
      });
    }

    if (post.is_public && (req.user || post.user_id === req.user.id))
      bot(post.id);

    const comments = await Comment.findByPostId(post.id);

    res.render("posts/view", {
      post,
      comments,
      user: req.user,
      title: post.title,
    });
  } catch (error) {
    console.error("게시물 조회 오류:", error);
    res.status(500).render("error", {
      message: "게시물을 불러오는 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const getEditForm = async (req, res) => {
  try {
    const post = await Post.findById(req.params.id);
    if (!post) {
      return res.status(404).render("error", {
        message: "게시물을 찾을 수 없습니다.",
        user: req.user,
      });
    }

    if (post.user_id !== req.user.id) {
      return res.status(403).render("error", {
        message: "이 게시물을 수정할 권한이 없습니다.",
        user: req.user,
      });
    }

    res.render("posts/edit", {
      post,
      user: req.user,
      title: "게시물 수정",
    });
  } catch (error) {
    console.error("게시물 수정 폼 오류:", error);
    res.status(500).render("error", {
      message: "게시물 수정 폼을 불러오는 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const updatePost = async (req, res) => {
  try {
    const { title, content, isPublic } = req.body;

    if (!title || !content) {
      return res.status(400).render("posts/edit", {
        errors: [{ msg: "제목과 내용을 모두 입력해주세요." }],
        post: req.body,
        user: req.user,
        title: "게시물 수정",
      });
    }

    const post = await Post.findById(req.params.id);
    if (!post) {
      return res.status(404).render("error", {
        message: "게시물을 찾을 수 없습니다.",
        user: req.user,
      });
    }

    if (post.user_id !== req.user.id) {
      return res.status(403).render("error", {
        message: "이 게시물을 수정할 권한이 없습니다.",
        user: req.user,
      });
    }

    await Post.update(req.params.id, title, content, isPublic === "on");
    res.redirect(`/posts/${req.params.id}`);
  } catch (error) {
    console.error("게시물 수정 오류:", error);
    res.status(500).render("error", {
      message: "게시물 수정 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const deletePost = async (req, res) => {
  try {
    const post = await Post.findById(req.params.id);
    if (!post) {
      return res.status(404).render("error", {
        message: "게시물을 찾을 수 없습니다.",
        user: req.user,
      });
    }

    if (post.user_id !== req.user.id) {
      return res.status(403).render("error", {
        message: "이 게시물을 삭제할 권한이 없습니다.",
        user: req.user,
      });
    }

    await Post.delete(req.params.id);
    res.redirect("/posts");
  } catch (error) {
    console.error("게시물 삭제 오류:", error);
    res.status(500).render("error", {
      message: "게시물 삭제 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const createComment = async (req, res) => {
  try {
    const { content } = req.body;
    const postId = req.params.id;

    if (!content) {
      return res.status(400).json({ message: "댓글 내용을 입력해주세요." });
    }

    const post = await Post.findById(postId);
    if (!post) {
      return res.status(404).json({ message: "게시물을 찾을 수 없습니다." });
    }

    if (!post.is_public && (!req.user || post.user_id !== req.user.id)) {
      return res
        .status(403)
        .json({ message: "이 게시물에 댓글을 작성할 권한이 없습니다." });
    }

    await Comment.create(content, postId, req.user.id);
    res.redirect(`/posts/${postId}`);
  } catch (error) {
    console.error("댓글 작성 오류:", error);
    res.status(500).render("error", {
      message: "댓글 작성 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

const deleteComment = async (req, res) => {
  try {
    const { id: postId, commentId } = req.params;
    const success = await Comment.delete(commentId, req.user.id);

    if (!success) {
      return res.status(404).render("error", {
        message: "댓글을 찾을 수 없거나 삭제할 권한이 없습니다.",
        user: req.user,
      });
    }

    res.redirect(`/posts/${postId}`);
  } catch (error) {
    console.error("댓글 삭제 오류:", error);
    res.status(500).render("error", {
      message: "댓글 삭제 중 오류가 발생했습니다.",
      user: req.user,
    });
  }
};

module.exports = {
  getPosts,
  getCreateForm,
  createPost,
  getPost,
  getEditForm,
  updatePost,
  deletePost,
  createComment,
  deleteComment,
};
