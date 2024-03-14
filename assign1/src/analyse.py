import os
import pandas as pd

OUTPUT_PATH = "output/"

def get_all_files(output_path, lang = 'cpp'):
    """
    Traverse through a given directory and its subdirectories to find all CSV files
    that contain 'cpp' in their filenames.
    """
    paths = []
    for root, dirs, files in os.walk(output_path):
        for file in files:
            if file.endswith('.csv') and lang in file:
                file_path = os.path.join(root, file)
                print(f"Found: {file_path}")
                paths.append(file_path)
    return paths

def get_merged_DF(files):
    all_data = []
    for file_path in files:
        try:
            data = pd.read_csv(file_path)
            all_data.append(data)
        except Exception as e:
            print(f"Error reading {file_path}: {e}")

    if not all_data:
        return pd.DataFrame()
        
    return all_data

def calc_mean_cpp(files):
    """
    Calculate the mean values for each unique combination ['Operation', 'Dimension', 'BlockSize', 'N_Threads'] 
    in a collection of CSV files.
    """
    all_data = get_merged_DF(files)

    combined_data = pd.concat(all_data)
    mean_data = combined_data.groupby(['Operation', 'Dimension', 'BlockSize', 'N_Threads']).mean().reset_index()
    
    mean_data['Time'] = mean_data['Time'].round(6)
    mean_data['Gflops'] = mean_data['Gflops'].round(4)
    mean_data['L1_DCM'] = mean_data['L1_DCM'].astype(int)
    mean_data['L2_DCM'] = mean_data['L2_DCM'].astype(int)
    mean_data['N_Threads'] = mean_data['N_Threads'].astype(int)

    mean_data['Speedup'] = 'N/A'
    mean_data['Efficiency'] = 'N/A'

    single_thread_data = mean_data[mean_data['N_Threads'] == 1].set_index(['Operation', 'Dimension', 'BlockSize'])
    
    for index, row in mean_data.iterrows():
        if int(row['N_Threads']) != 1:
            
            if int(row['Operation']) in [4,5]:
                key = (2, int(row['Dimension']), int(row['BlockSize']))
            else:
                key = (3, int(row['Dimension']), int(row['BlockSize']))
                
            if key in single_thread_data.index:
                single_thread_time = single_thread_data.loc[key, 'Time']
                speedup = single_thread_time / row['Time']
                efficiency = speedup / row['N_Threads']

                mean_data.at[index, 'Speedup'] = round(speedup, 6)
                mean_data.at[index, 'Efficiency'] = round(efficiency, 6)

    return mean_data

def calc_mean_java(files):
    """
    Calculate the mean values for each unique combination ['Operation', 'Dimension', 'BlockSize'] 
    in a collection of CSV files.
    """
    all_data = get_merged_DF(files)

    combined_data = pd.concat(all_data)
    mean_data = combined_data.groupby(['Operation', 'Dimension', 'BlockSize']).mean().reset_index()
    
    mean_data['Time'] = mean_data['Time'].round(6)
    mean_data['Gflops'] = mean_data['Gflops'].round(4)

    return mean_data

def save_in_new_csv(file_name, df):
    """
    Save a DataFrame to a CSV file.
    """
    df.to_csv(OUTPUT_PATH + file_name, index=False)
    print(f"Results saved to {file_name}")


if __name__ == "__main__":
    files = get_all_files(OUTPUT_PATH)
    results = calc_mean_cpp(files)
    save_in_new_csv("aggregated_results_cpp.csv", results)

    # files = get_all_files(OUTPUT_PATH, 'java')
    # results = calc_mean_java(files)
    # save_in_new_csv("aggregated_results_java.csv", results)