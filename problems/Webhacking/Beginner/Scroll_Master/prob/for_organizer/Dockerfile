# 베이스 이미지 설정
FROM php:8.0-apache

# 작업 디렉토리 설정
WORKDIR /var/www/html

# 로컬 파일을 컨테이너로 복사
COPY ./html /var/www/html

# 포트 설정
EXPOSE 80

# 컨테이너 실행 시 Apache 웹 서버 실행
CMD ["apache2-foreground"]
