Here's your text formatted in Markdown:

# A1. Oscilloscope:

## Arduino: 
Timed sampling of up to 8 channels on the ADC, and streaming on PC.

### Configurations (from PC):
- Which channel(s) are being sampled
- The sampling frequency

### Operating Modes:
- **Continuous sampling**: Each sample is sent to the PC as it becomes ready.
- **Buffered mode**: A set of samples are stored locally and then sent in bulk.

### Additional Features:
- Implementation of "trigger" (a condition that decides when to start sampling, in buffered mode).
- Serial communication should be asynchronous (use an interrupt on the UART).

## PC: 
Host program controlling the oscilloscope. Output might be done on text files and displayed with gnuplot.