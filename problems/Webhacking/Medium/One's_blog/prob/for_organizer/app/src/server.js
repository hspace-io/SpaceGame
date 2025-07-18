require("dotenv").config();
const express = require("express");
const cors = require("cors");
const path = require("path");
const cookieParser = require("cookie-parser");
const methodOverride = require("method-override");
const { checkAuth, requireAuth } = require("./middleware/auth.middleware");
const indexRoutes = require("./routes/index.routes");
const authRoutes = require("./routes/auth.routes");
const postRoutes = require("./routes/post.routes");
const initDatabase = require("./database/init");

require("dotenv").config();

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(cookieParser());

app.use((req, res, next) => {
  res.setHeader("Cross-Origin-Embedder-Policy", "require-corp");
  res.setHeader("Cross-Origin-Opener-Policy", "same-origin");
  res.setHeader("Cross-Origin-Resource-Policy", "same-origin");
  res.setHeader("X-Frame-Options", "DENY");
  res.setHeader("X-Content-Type-Options", "nosniff");
  res.setHeader("Cache-Control", "no-store");

  next();
});

app.use(methodOverride("_method"));
app.use(express.static(path.join(__dirname, "../public")));

app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "../public/views"));

app.use(checkAuth);

app.use("/", indexRoutes);
app.use("/auth", authRoutes);
app.use("/posts", requireAuth, postRoutes);

app.use((req, res) => {
  res.status(404).render("error", { message: "페이지를 찾을 수 없습니다." });
});

app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).render("error", { message: "서버 오류가 발생했습니다." });
});

initDatabase();
app.listen(PORT, () => {
  console.log(`Server is Running at http://localhost:${PORT}`);
});
