from typing import List
from bs4 import BeautifulSoup
from dataclasses import dataclass

@dataclass
class Sample:
    input_number: int
    input_text: str
    output_number: int
    output_text: str

@dataclass
class ProblemStatement:
    title: str
    time_limit: str
    memory_limit: str
    description: str
    input_spec: str
    output_spec: str
    samples: List[Sample]

def convert_math_elements(element):
    element_copy = BeautifulSoup(str(element), 'html.parser')
    
    for sup in element_copy.find_all('sup'):
        sup_content = sup.get_text()
        sup.replace_with(f'^{sup_content}')
    
    for sub in element_copy.find_all('sub'):
        sub_content = sub.get_text()
        sub.replace_with(f'_{sub_content}')
    
    for italic in element_copy.find_all('i'):
        italic_content = italic.get_text()
        italic.replace_with(italic_content)
    
    text = element_copy.get_text(separator=' ', strip=True)
    text = text.replace(' ', ' ') 
    
    return text

def parse_problem_statement(html_content):
    soup = BeautifulSoup(html_content, 'html.parser')
    
    problem_statement = soup.find('div', class_='problem-statement')
    
    if not problem_statement:
        return "Error"
    
    title = problem_statement.find('div', class_='title').get_text(separator=' ', strip=True)
    
    time_limit = problem_statement.find('div', class_='time-limit').get_text(separator=' ', strip=True)
    memory_limit = problem_statement.find('div', class_='memory-limit').get_text(separator=' ', strip=True)
    
    header_div = problem_statement.find('div', class_='header')
    description_div = header_div.find_next_sibling('div')
    description = convert_math_elements(description_div) if description_div else ""
    
    input_spec_div = problem_statement.find('div', class_='input-specification')
    input_spec = convert_math_elements(input_spec_div) if input_spec_div else ""
    
    output_spec_div = problem_statement.find('div', class_='output-specification')
    output_spec = convert_math_elements(output_spec_div) if output_spec_div else ""
    
    samples = []
    sample_test = problem_statement.find('div', class_='sample-test')
    if sample_test:
        inputs = sample_test.find_all('div', class_='input')
        outputs = sample_test.find_all('div', class_='output')
        
        for i in range(len(inputs)):
            sample_input = inputs[i].find('pre').get_text('\n', strip=True)
            sample_output = outputs[i].find('pre').get_text('\n', strip=True)
            samples.append(Sample(
                input_number=i+1,
                input_text=sample_input,
                output_number=i+1,
                output_text=sample_output
            ))
    
    note = ""
    note_div = problem_statement.find('div', class_='note')
    if note_div:
        note = convert_math_elements(note_div)
    
    result = ProblemStatement(
        title=title, 
        time_limit=time_limit, 
        memory_limit=memory_limit, 
        description=description, 
        input_spec=input_spec, 
        output_spec=output_spec, 
        samples=samples
    )
    return result

if __name__ == "__main__":
    with open('/home/kercoza/taskgen/Tasks_Generator/codeforces_problem.html', 'r') as file:
        content = file.read()
    pr = parse_problem_statement(content)
    print(pr.output_spec)