from urllib.request import urlopen, Request

def get_problem_html(url):
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) '
                      'AppleWebKit/537.36 (KHTML, like Gecko) '
                      'Chrome/91.0.4472.124 Safari/537.36'
    }
    try:
        with urlopen(Request(url, headers=headers), timeout=10) as response:
            return response.read().decode('utf-8')
    except Exception as e:
        print(f"Error loading page: {e}")
        return None

def save_to_file(content, filename):
    try:
        with open(filename, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    except Exception as e:
        print(f"Error saving file: {e}")
        return False

def main():
    html_content = get_problem_html("https://codeforces.com/problemset/problem/1918/A")
    
    if html_content:
        filename = 'codeforces_problem.html'
        if save_to_file(html_content, filename):
            print(f"Saved to: {filename}")
        else:
            print("Problem with saving to file")
    else:
        print("Error loading HTML")

if __name__ == "__main__":
    main()