#!/bin/bash

echo "Available Problems:"
find challenges -mindepth 3 -maxdepth 3 -type d | sed 's|challenges/||' | sort |nl

echo ""
read -p "Enter problem number to build : " -a nums

mapfile -t all_paths < <(find challenges -mindepth 3 -maxdepth 3 -type d | sort)

echo ${all_paths[nums-1]}
cd ${all_paths[nums-1]}

found_dir=$(find . -type d -name "for_organizer" | head -n 1)

if [ -n "$found_dir" ]; then
  cd "$found_dir"
else
  echo "No"
fi

docker-compose build
docker-compose up -d
