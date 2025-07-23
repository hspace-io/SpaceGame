const db = require("../database/sqlite.database");
const { v4: uuidv4 } = require("uuid");

class Post {
  static async create(title, content, isPublic, userId) {
    const id = uuidv4();

    return new Promise((resolve, reject) => {
      db.run(
        "INSERT INTO posts (id, title, content, is_public, user_id) VALUES (?, ?, ?, ?, ?)",
        [id, title, content, isPublic ? 1 : 0, userId],
        function (err) {
          if (err) reject(err);

          db.get("SELECT * FROM posts WHERE id = ?", [id], function (err, row) {
            if (err) return reject(err);
            resolve(row.id);
          });
        }
      );
    });
  }

  static async findById(id) {
    return new Promise((resolve, reject) => {
      db.get(
        `SELECT p.*, u.username, u.id as author_id
                FROM posts p 
                JOIN users u ON p.user_id = u.id 
                WHERE p.id = ?`,
        [id],
        (err, row) => {
          if (err) reject(err);
          if (row) {
            row.author = {
              _id: row.author_id,
              username: row.username,
            };
            delete row.author_id;
            delete row.username;
          }
          resolve(row);
        }
      );
    });
  }

  static async findAll(id) {
    return new Promise((resolve, reject) => {
      const query = `
                SELECT p.*, u.username, u.id as author_id
                FROM posts p 
                JOIN users u ON p.user_id = u.id 
                WHERE p.user_id = ?
                ORDER BY p.created_at DESC
            `;

      db.all(query, [id], (err, rows) => {
        if (err) return reject(err);
        if (rows) {
          rows = rows.map((row) => {
            row.author = {
              _id: row.author_id,
              username: row.username,
            };
            delete row.author_id;
            delete row.username;
            return row;
          });
        }
        resolve(rows);
      });
    });
  }

  static async update(id, title, content, isPublic) {
    return new Promise((resolve, reject) => {
      db.run(
        `UPDATE posts 
                SET title = ?, content = ?, is_public = ?, updated_at = CURRENT_TIMESTAMP 
                WHERE id = ?`,
        [title, content, isPublic ? 1 : 0, id],
        function (err) {
          if (err) reject(err);
          resolve(this.changes > 0);
        }
      );
    });
  }

  static async delete(id) {
    return new Promise((resolve, reject) => {
      db.run("DELETE FROM posts WHERE id = ?", [id], function (err) {
        if (err) reject(err);
        resolve(this.changes > 0);
      });
    });
  }

  static async findByUserId(userId) {
    return new Promise((resolve, reject) => {
      db.all(
        `SELECT p.*, u.username, u.id as author_id
                FROM posts p 
                JOIN users u ON p.user_id = u.id 
                WHERE p.user_id = ? 
                ORDER BY p.created_at DESC`,
        [userId],
        (err, rows) => {
          if (err) reject(err);
          if (rows) {
            rows = rows.map((row) => {
              row.author = {
                _id: row.author_id,
                username: row.username,
              };
              delete row.author_id;
              delete row.username;
              return row;
            });
          }
          resolve(rows);
        }
      );
    });
  }
}

module.exports = Post;
