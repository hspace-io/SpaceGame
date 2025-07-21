const db = require("../database/sqlite.database");
const { v4: uuidv4 } = require("uuid");

class Comment {
  static async create(content, postId, userId) {
    return new Promise((resolve, reject) => {
      db.run(
        "INSERT INTO comments (id, content, post_id, user_id) VALUES (?, ?, ?, ?)",
        [uuidv4(), content, postId, userId],
        function (err) {
          if (err) reject(err);
          resolve(this.lastID);
        }
      );
    });
  }

  static async findByPostId(postId) {
    return new Promise((resolve, reject) => {
      db.all(
        `SELECT c.*, u.username 
                FROM comments c 
                JOIN users u ON c.user_id = u.id 
                WHERE c.post_id = ? 
                ORDER BY c.created_at ASC`,
        [postId],
        (err, rows) => {
          if (err) reject(err);
          resolve(rows);
        }
      );
    });
  }

  static async delete(id, userId) {
    return new Promise((resolve, reject) => {
      db.run(
        "DELETE FROM comments WHERE id = ? AND user_id = ?",
        [id, userId],
        function (err) {
          if (err) reject(err);
          resolve(this.changes > 0);
        }
      );
    });
  }
}

module.exports = Comment;
