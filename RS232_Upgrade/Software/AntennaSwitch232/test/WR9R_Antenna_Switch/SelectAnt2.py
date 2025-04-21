import requests

url = "http://192.168.1.179/TWO"

response = requests.get(url)

print(response.text)
