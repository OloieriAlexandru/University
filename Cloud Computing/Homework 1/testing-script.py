import requests
import argparse
import time
import concurrent.futures

URL_BASE = "http://127.0.0.1:4444"
URL_RANDOM_IMAGE = '{}/api/v1/random-image'.format(URL_BASE)
URL_RANDOM_FACT = '{}/api/v1/random-fact'.format(URL_BASE)
URL_RANDOM_FACT_IMAGE = '{}/api/v1/fact-image'.format(URL_BASE)

COMMON_HEADERS = {'Content-Type': "application/json",
                  'Accept': "application/json"}


def endpoint_call_random_image():
    return requests.get(URL_RANDOM_IMAGE, headers=COMMON_HEADERS)


def endpoint_call_random_fact():
    return requests.post(URL_RANDOM_FACT, headers=COMMON_HEADERS, data={'lang': 'en'})


def endpoint_call_random_image_fact():
    return requests.post(URL_RANDOM_FACT_IMAGE, headers=COMMON_HEADERS, data={})


def get_seconds_string(s):
    return '{:.4f} seconds'.format(s)


ENDPOINT_MAPPING = {
    "random-image": endpoint_call_random_image,
    "random-fact": endpoint_call_random_fact,
    "random-image-fact": endpoint_call_random_image_fact
}


def get_cmd_commands():
    parser = argparse.ArgumentParser(
        description="Send concurrent requests in batches to a web server")

    possible_endpoints = []
    for key in ENDPOINT_MAPPING:
        possible_endpoints.append(key)

    parser.add_argument('--batches', '-b', type=int, default=10)
    parser.add_argument('--batch_size', '-bs', type=int, default=50)
    parser.add_argument('--endpoint', '-e', type=str,
                        default="random-image-fact", choices=possible_endpoints)
    parser.add_argument('--max_workers', '-mw', type=int, default=5)

    return parser


def test_batch(args):
    latencies = []
    successes = []
    with concurrent.futures.ThreadPoolExecutor(args.max_workers) as tp_executor:
        futures = []
        for _ in range(args.batch_size):
            futures.append(tp_executor.submit(ENDPOINT_MAPPING[args.endpoint]))

        for request in futures:
            req_result = request.result()
            latencies.append(req_result.elapsed.total_seconds())
            successes.append(200 <= req_result.status_code < 300)

    return (latencies, successes)


def print_lat_succ_info(latencies, successes):
    print('Successful requests: {}/{}'.format(sum(successes), len(successes)))
    print('Average latency: {:.4f} seconds'.format(
        sum(latencies)/len(latencies)))
    print('Max latency: {:.4f} seconds'.format(max(latencies)))
    print('Min latency: {:.4f} seconds'.format(min(latencies)))


def test_endpoint(args):
    agg_latencies = []
    agg_successes = []
    for batch_idx in range(1, args.batches + 1):
        print('-----------------')
        print("Batch {}".format(batch_idx))
        batch_start_time = time.perf_counter()

        latencies, successes = test_batch(args)
        agg_latencies += latencies
        agg_successes += successes

        batch_end_time = time.perf_counter()
        print('Total batch execution time: {}'.format(
            get_seconds_string(batch_end_time - batch_start_time)))
        print_lat_succ_info(latencies, successes)

    print('---------------------------')
    print('Aggregated results')
    print_lat_succ_info(agg_latencies, agg_successes)


if __name__ == '__main__':
    args = get_cmd_commands().parse_args()
    test_endpoint(args)
