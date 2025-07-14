# 베이스 이미지
FROM python:3.11-slim

# 작업 디렉토리 생성
WORKDIR /app

# 애플리케이션 파일 복사
COPY . .

# 의존성 설치
RUN pip install --no-cache-dir flask

# DB 초기화
RUN python3 -c "from db import init_db; init_db()"

# 포트 오픈
EXPOSE 5000

# 실행
CMD ["python3", "app.py"]
