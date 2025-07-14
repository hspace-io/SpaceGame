# 문제이름

# Usage
- 공란

# Concept
- 문제 컨셉 1-Day(CVE-2024-38475, CVE-2024-38474) + php Segfault

# Writeup

1. ``FROM php:7.0-apache`` PHP Version이 예전 7.0 버전인 것을 알 수 있음
2. dir.php에서 file_get_contents 함수를 통해 파일을 읽을 수 있는 기능이 존재함.
