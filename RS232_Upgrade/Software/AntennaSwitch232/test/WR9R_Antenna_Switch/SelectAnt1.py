import requests

url = "http://192.168.1.179/ONE"

response = requests.get(url)

if response.status_code == 200:
    print("Success!")
elif response.status_code == 404:
    print("Resource not found!")
else:
    print("Error!")

# print(response.text)
