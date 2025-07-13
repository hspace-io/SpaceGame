#!/bin/bash

input_file="table_sample.md"
output_file="table.md"
link_defs="links.md"

> "$output_file"
> "$link_defs"

while IFS= read -r line; do
  if [[ "$line" =~ ^\| ]]; then
    IFS='|' read -ra fields <<< "$line"

    field_type=$(echo "${fields[1]}" | xargs)
    field_level=$(echo "${fields[2]}" | xargs)
    field_name=$(echo "${fields[3]}" | xargs)

    if [[ -n "$field_name" ]]; then
      link_text="[problem][${field_name} prob] [solution][${field_name} sol]"
      fields[4]=" $link_text "

      new_line=$(IFS='|'; echo "${fields[*]}")
      echo "$new_line" >> "$output_file"

      echo "[${field_name} prob]: ./problems/${field_type}/${field_level}/${field_name}" >> "$link_defs"
      echo "[${field_name} sol]: ./solutions/${field_type}/${field_level}/${field_name}" >> "$link_defs"
      echo "" >> "$link_defs"
    else
      echo "$line" >> "$output_file"
    fi
  else
    echo "$line" >> "$output_file"
  fi
done < "$input_file"

cat "$output_file" "$link_defs" > final_output.md


# sed -i 's/\r$//' filename