def process_file(input_path, cleaned_output_path):
    timestamps = []
    outputs = []
    current_ts = None

    with open(input_path, 'r') as f:
        for line in f:
            line = line.rstrip('\n')
            # Skip TLAST lines entirely
            if line.startswith('TLAST'):
                continue
            # Timestamp lines: record but don’t write out
            if line.startswith('T '):
                # e.g. line = "T 6624 ns"
                current_ts = line[2:].strip()  # "6624 ns"
                continue
            # Otherwise it’s an output line
            outputs.append(line)
            timestamps.append(current_ts)

    # Save the cleaned file (no T or TLAST lines)
    with open(cleaned_output_path, 'w') as f:
        for out in outputs:
            f.write(out + '\n')

    # Get the 26th timestamp (1-based)
    ts_26 = timestamps[25] if len(timestamps) >= 26 else None
    return ts_26
    

if __name__ == "__main__":
    import sys
    ts_26th = process_file(sys.argv[1], sys.argv[2])
    if ts_26th:
        print(f"Timestamp of the 26th output: {ts_26th}")
    else:
        print("Fewer than 26 outputs found.")
