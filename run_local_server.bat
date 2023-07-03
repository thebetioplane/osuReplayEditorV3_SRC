@echo off
rem To use set update_url = http://localhost:8000/ in config.txt
rem
cd distro
start "" python -m http.server 8000
