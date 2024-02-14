echo "Running $(clang-format.exe --version)..."
find ./Solar -type f \
  \( -iname '*.cpp' -o -iname '*.h' \) -print0 \
  | xargs -0 clang-format.exe -style=file -i
find ./Solarium -type f \
  \( -iname '*.cpp' -o -iname '*.h' \) -print0 \
  | xargs -0 clang-format.exe -style=file -i

echo "Running cpplint..."
find ./Solar/* -maxdepth 0 -type d -print0 \
  | xargs -P0 -n1 -0 cpplint --recursive --quiet
find ./Solarium/* -maxdepth 0 -type d -print0 \
  | xargs -P0 -n1 -0 cpplint --recursive --quiet