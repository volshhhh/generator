from bs4 import BeautifulSoup
def parse_problem_statement(html_file):
    with open(html_file, 'r', encoding='utf-8') as file:
        html_content = file.read()
    
    soup = BeautifulSoup(html_content, 'html.parser')
    
    problem_statement = soup.find('div', class_='problem-statement')
    
    if not problem_statement:
        return "Error"
    
    title = problem_statement.find('div', class_='title').get_text(strip=True)
    
    time_limit = problem_statement.find('div', class_='time-limit').get_text(strip=True)
    memory_limit = problem_statement.find('div', class_='memory-limit').get_text(strip=True)
    
    description = problem_statement.find('div').find_next_sibling('div').get_text(strip=True)
    
    input_spec = problem_statement.find('div', class_='input-specification').get_text(strip=True)
    output_spec = problem_statement.find('div', class_='output-specification').get_text(strip=True)
    
    samples = []
    sample_test = problem_statement.find('div', class_='sample-test')
    if sample_test:
        inputs = sample_test.find_all('div', class_='input')
        outputs = sample_test.find_all('div', class_='output')
        
        for i in range(len(inputs)):
            sample_input = inputs[i].find('pre').get_text('\n', strip=True)
            sample_output = outputs[i].find('pre').get_text('\n', strip=True)
            samples.append((f"Input example {i+1}", sample_input, f"Output example {i+1}", sample_output))
    
    note = ""
    note_div = problem_statement.find('div', class_='note')
    if note_div:
        note = note_div.get_text(strip=True)
    
    result = f"""
Title: {title}
{time_limit}
{memory_limit}

Description:
{description}

Input:
{input_spec}

Output:
{output_spec}
"""
    
    for sample in samples:
        result += f"\n{sample[0]}:\n{sample[1]}\n{sample[2]}:\n{sample[3]}\n"
    
    if note:
        result += f"\nNote:\n{note}"
    
    return result

if __name__ == "__main__":
    html_file = "codeforces_problem.html"
    problem_text = parse_problem_statement(html_file)
    print(problem_text)
