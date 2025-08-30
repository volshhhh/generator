import grpc
from concurrent import futures
import time
import connection_pb2
import connection_pb2_grpc

from include.fullhtml import get_problem_html
from include.parser import parse_problem_statement

class ParsingServicer(connection_pb2_grpc.ParsingServicer):
    def ParseUri(self, request, context):

        with open('/home/kercoza/taskgen/parser/server/include/codeforces_problem.html', 'r', encoding='utf-8') as file:
            problem_html = file.read()
        # Problem = parse_problem_statement(get_problem_html(request.uri))

        # print(Problem.description)
        # response = connection_pb2.Response(
        #         name=Problem.title,
        #         text=Problem.description
        #     )
        
        Problem = parse_problem_statement(problem_html)
        
        response = connection_pb2.Response(
            title=Problem.title,
            time_limit=Problem.time_limit,
            memory_limit=Problem.memory_limit,
            description=Problem.description,
            input_spec=Problem.input_spec,
            output_spec=Problem.output_spec
        )

        return response

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    connection_pb2_grpc.add_ParsingServicer_to_server(ParsingServicer(), server)
    server.add_insecure_port('[::]:50051')
    
    
    server.start()
    print("Server started on port 50051")
    
    try:
        while True:
            time.sleep(1000)
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == '__main__':
    serve()