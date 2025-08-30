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

def parse_problem_statement(html_content):
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
    
    result = ProblemStatement(title=title, 
                              time_limit=time_limit, 
                              memory_limit=memory_limit, 
                              description=description, 
                              input_spec=input_spec, 
                              output_spec=output_spec, samples=samples)
    return result