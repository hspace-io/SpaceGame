const express = require("express");
const router = express.Router();
const postController = require("../controllers/post.controller");

router.get("/", postController.getPosts);
router.get("/create", postController.getCreateForm);
router.get("/:id", postController.getPost);
router.get("/:id/edit", postController.getEditForm);

router.post("/", postController.createPost);
router.post("/:id/edit", postController.updatePost);
router.post("/:id/delete", postController.deletePost);
router.post("/:id/comments", postController.createComment);

module.exports = router;
