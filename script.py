from scapy.all import rdpcap, Raw
import struct

def parse_sample_data(byte_data, num_channels):
    if num_channels == 1:
        return [(byte_data >> i) & 1 for i in range(8)]  # 获取每个位
    else:
        return [[(byte_data >> (i * num_channels + ch)) & 1 for i in range(8 // num_channels)] for ch in range(num_channels)]

def pcapng_to_bin(pcapng_file, bin_file, num_channels):
    packets = rdpcap(pcapng_file)
    
    with open(bin_file, 'wb') as binfile:
        for packet in packets:
            if Raw in packet:
                raw_data = packet[Raw].load
                for byte_data in raw_data:
                    parsed_samples = parse_sample_data(byte_data, num_channels)
                    
                    if num_channels == 1:
                        # 将单通道数据直接写入 .bin 文件
                        for sample in parsed_samples:
                            binfile.write(struct.pack("B", sample))
                    else:
                        # 多通道情况下，组装各通道的位到一个字节
                        for i in range(len(parsed_samples[0])):
                            sample_byte = 0
                            for ch in range(num_channels):
                                sample_byte |= (parsed_samples[ch][i] << (num_channels - ch - 1))
                            binfile.write(struct.pack("B", sample_byte))

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="Convert PCAPNG file to binary file.")
    parser.add_argument("pcapng_file", type=str, help="Path to the input PCAPNG file")
    parser.add_argument("bin_file", type=str, help="Path to the output binary file")
    parser.add_argument("num_channels", type=int, help="Number of channels (1 for single channel, >1 for multi-channel)")
    
    args = parser.parse_args()
    
    pcapng_to_bin(args.pcapng_file, args.bin_file, args.num_channels)
