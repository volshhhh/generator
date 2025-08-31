from typing import List
from bs4 import BeautifulSoup
from dataclasses import dataclass
import re

@dataclass
class Sample_data:
    input_data: str
    output_data: str

@dataclass
class ProblemStatement:
    title: str
    time_limit: str
    memory_limit: str
    description: str
    input_spec: str
    output_spec: str
    samples: List[Sample_data]

def convert_math_elements(element):
    if not element:
        return ""
    
    element_copy = BeautifulSoup(str(element), 'html.parser')
    
    # Обработка математических выражений с классом tex-span
    for tex_span in element_copy.find_all('span', class_='tex-span'):
        math_content = convert_tex_span_to_latex(tex_span)
        tex_span.replace_with(f'${math_content}$')
    
    # Обработка отдельных математических тегов
    for sup in element_copy.find_all('sup'):
        sup_content = sup.get_text()
        sup.replace_with(f'^{{{sup_content}}}')
    
    for sub in element_copy.find_all('sub'):
        sub_content = sub.get_text()
        sub.replace_with(f'_{{{sub_content}}}')
    
    for italic in element_copy.find_all('i'):
        italic_content = italic.get_text()
        italic.replace_with(italic_content)
    
    # Получение текста и очистка
    text = element_copy.get_text(separator=' ', strip=True)
    
    # Замена специальных символов на LaTeX эквиваленты
    text = text.replace('≤', '\\leq')
    text = text.replace('≥', '\\geq')
    text = text.replace('≠', '\\neq')
    text = text.replace(' ', ' ')
    
    # Удаление лишних пробелов
    text = re.sub(r'\s+', ' ', text)
    
    return text

def convert_tex_span_to_latex(tex_span):
    """Конвертирует span с классом tex-span в LaTeX формат"""
    soup = BeautifulSoup(str(tex_span), 'html.parser')
    
    # Обрабатываем нижние индексы
    for sub in soup.find_all('sub', class_='lower-index'):
        sub_content = sub.get_text()
        sub.replace_with(f'_{{{sub_content}}}')
    
    # Обрабатываем верхние индексы
    for sup in soup.find_all('sup', class_='upper-index'):
        sup_content = sup.get_text()
        sup.replace_with(f'^{{{sup_content}}}')
    
    # Обрабатываем курсив (переменные)
    for italic in soup.find_all('i'):
        italic_content = italic.get_text()
        italic.replace_with(italic_content)
    
    # Получаем текст и заменяем специальные символы
    text = soup.get_text()
    text = text.replace('≤', '\\leq')
    text = text.replace('≥', '\\geq')
    text = text.replace('≠', '\\neq')
    text = text.replace(' ', ' ')
    
    return text

def parse_problem_statement(html_content):
    soup = BeautifulSoup(html_content, 'html.parser')
    
    problem_statement = soup.find('div', class_='problem-statement')
    
    if not problem_statement:
        return None
    
    # Извлечение заголовка
    title_elem = problem_statement.find('div', class_='title')
    title = title_elem.get_text(strip=True) if title_elem else ""
    
    # Лимиты
    time_limit_elem = problem_statement.find('div', class_='time-limit')
    time_limit = time_limit_elem.get_text(strip=True) if time_limit_elem else ""
    
    memory_limit_elem = problem_statement.find('div', class_='memory-limit')
    memory_limit = memory_limit_elem.get_text(strip=True) if memory_limit_elem else ""
    
    # Описание - более надежный способ поиска
    description = ""
    # Ищем все div без классов после header
    for div in problem_statement.find_all('div'):
        classes = div.get('class', [])
        if not classes and div.find_previous_sibling('div', class_='header'):
            description = convert_math_elements(div)
            break
    
    # Если не нашли описание, попробуем альтернативный способ
    if not description:
        header = problem_statement.find('div', class_='header')
        if header:
            next_div = header.find_next_sibling('div')
            if next_div and not next_div.get('class'):
                description = convert_math_elements(next_div)
    
    # Спецификации ввода/вывода
    input_spec_elem = problem_statement.find('div', class_='input-specification')
    input_spec = convert_math_elements(input_spec_elem) if input_spec_elem else ""
    
    output_spec_elem = problem_statement.find('div', class_='output-specification')
    output_spec = convert_math_elements(output_spec_elem) if output_spec_elem else ""
    
    # Примеры
    samples = []
    sample_test = problem_statement.find('div', class_='sample-test')
    if sample_test:
        inputs = sample_test.find_all('div', class_='input')
        outputs = sample_test.find_all('div', class_='output')
        
        for i in range(min(len(inputs), len(outputs))):
            input_pre = inputs[i].find('pre')
            output_pre = outputs[i].find('pre')
            
            if input_pre and output_pre:
                sample_input = input_pre.get_text('\n').strip()
                sample_output = output_pre.get_text('\n').strip()
                samples.append(Sample_data(
                    input_data=sample_input,
                    output_data=sample_output
                ))
    
    return ProblemStatement(
        title=title, 
        time_limit=time_limit, 
        memory_limit=memory_limit, 
        description=description, 
        input_spec=input_spec, 
        output_spec=output_spec, 
        samples=samples
    )

# if __name__ == "__main__":
#     with open('/home/kercoza/taskgen/Tasks_Generator/server/include/codeforces_problem.html', 'r', encoding='utf-8') as file:
#         content = file.read()
#     pr = parse_problem_statement(content)
