#!/bin/bash
#docker stop $(docker ps -aq)
#docker rm $(docker ps -aq)
#docker rmi -f $(docker images -aq)
#docker builder prune -af
#docker volume prune -f
#docker network prune -f

#docker-compose down --volumes --remove-orphans
#docker image prune -f


#!/bin/bash

echo "⚠️ Docker 환경 전체를 초기화합니다. 5초 내에 Ctrl+C로 취소할 수 있습니다..."
sleep 5

echo "🛑 모든 컨테이너 중지 및 삭제..."
docker stop $(docker ps -aq) 2>/dev/null
docker rm $(docker ps -aq) 2>/dev/null

echo "🗑 모든 이미지 삭제..."
docker rmi -f $(docker images -aq) 2>/dev/null

echo "🧹 빌더 캐시 삭제..."
docker builder prune -af

echo "📦 모든 볼륨 삭제..."
docker volume prune -f

echo "🌐 모든 네트워크 삭제..."
docker network prune -f

echo "🧼 dangling 이미지와 중간 캐시도 정리 중..."
docker system prune -af --volumes

echo "✅ Docker 초기화 완료"

