import matplotlib.pyplot as plt

def read_data(filename):
    data = {}
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split(': ')
            if len(parts) == 2:
                name, values = parts
                count, time = map(float, values.split())
                if name not in data:
                    data[name] = {'count': [], 'time': []}
                data[name]['count'].append(int(count))
                data[name]['time'].append(time)
    return data

def plot_data(data):
    plt.figure(figsize=(10, 6))
    for name, values in data.items():
        plt.plot(values['count'], values['time'], marker='o', linestyle='-', label=name)
    
    plt.xlabel('Count')
    plt.ylabel('Time Taken (seconds)')
    plt.title('Performance over Different Counts')
    
    all_times = [time for values in data.values() for time in values['time'] if time > 0]
    min_y = min(all_times) - 0.0001 if all_times else 0
    max_y = max(all_times) * 1.1 if all_times else 1
    plt.ylim(min_y, max_y)
    
    plt.legend()
    plt.grid()
    plt.show()

if __name__ == "__main__":
    filename = "Testing/preciseTimes.txt"
    data = read_data(filename)
    plot_data(data)
