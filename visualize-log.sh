for file in ./log/*
do
  dot -Tpng "$file" -o "$file.png"
done
