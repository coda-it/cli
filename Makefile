all: 
	g++ -std=c++17 -I $(HOME)/gowork/src/github.com/coda-it/cpputillib/include/ main.cpp -o main

.PHONY: fix
fix:
	clang-format -i -style=file *.cpp *.h

.PHONY: version
version:
	git tag $(V)
	./scripts/changelog.sh
	git add ./docs/changelogs/CHANGELOG_$(V).md
	./scripts/version.sh $(V)
	git add ./version.h
	git commit --allow-empty -m "Build $(V)"
	git tag --delete $(V)
	git tag $(V)	