from typing import List
import grpc
from concurrent import futures
import time
import connection_pb2
import connection_pb2_grpc

from include.fullhtml import get_problem_html
from include.parser import parse_problem_statement
from include.parser import Sample_data


def dataclass_samples_to_protobuf(data_samples: List[Sample_data]):
    from connection_pb2 import Sample
    
    return [
        Sample(
            input_data=sample.input_data,
            output_data=sample.output_data
        )
        for sample in data_samples
    ]



class ParsingServicer(connection_pb2_grpc.ParsingServicer):
    def ParseUri(self, request, context):
        # with open('/home/kercoza/taskgen/Tasks_Generator/server/include/codeforces_problem.html', 'r') as file:
        #     content = file.read()
        Problem = parse_problem_statement(get_problem_html(request.uri))
        # Problem = parse_problem_statement(content)
        # print(Problem.description)
        # response = connection_pb2.Response(
        #         name=Problem.title,
        #         text=Problem.description
        #     )
        
        # Problem = parse_problem_statement(problem_html)
        response = connection_pb2.Response(
            title=Problem.title,
            time_limit=Problem.time_limit,
            memory_limit=Problem.memory_limit,
            description=Problem.description,
            input_spec=Problem.input_spec,
            output_spec=Problem.output_spec,
            samples=dataclass_samples_to_protobuf(Problem.samples)
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