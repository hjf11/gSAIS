from random import randint

filenames = ['ecoli/ecoli1_1e06.fna', 'ecoli/ecoli2_1e06.fna', 'ecoli/ecoli3_1e06.fna', 
             'ecoli/ecoli4_1e06.fna', 'ecoli/ecoli5_1e06.fna', 'ecoli/ecoli6_1e06.fna',
             'ecoli/ecoli7_1e06.fna', 'ecoli/ecoli8_1e06.fna']
files = []
for name in filenames:
    files.append(open(name, 'r').readlines())

size = len(files[0]) - 1
for name, file in zip(filenames, files):
    for i in range(2, 6):
        idx = randint(0, size - 10**(i-1))
        new_name = name[:16] + str(i) + name[17:]
        with open(new_name, 'w') as f:
            f.write(file[0][0:-1] + ' 1e0' + str(i) + '\n')
            f.write(''.join(file[idx:idx+10**(i-1)]))

        new_name = name[:16] + str(i) + '_substrings' + name[17:]
        main_slice = file[idx:idx+10**(i-1)]
        main_slice_size = len(main_slice)
        with open(new_name, 'w') as f:
            f.write(file[0][0:-1] + ' 1e0' + str(i) + '_substrings' + '\n')
            for j in range(10):
                #subslice_size = 10
                subslice_size = randint(1, main_slice_size)
                subslice_idx = randint(0, main_slice_size - subslice_size)
                subslice = main_slice[subslice_idx : subslice_idx+subslice_size]
                f.write(''.join(subslice) + ',')

    new_name = name[:17] + '_substrings' + name[17:]
    main_slice = file
    main_slice_size = len(main_slice)
    with open(new_name, 'w') as f:
            f.write(file[0][0:-1] + ' 1e06' + '_substrings' + '\n')
            for j in range(10):
                #subslice_size = 10
                #subslice_size = randint(1, 5)
                subslice_size = randint(1, main_slice_size)
                subslice_idx = randint(0, main_slice_size - subslice_size)
                subslice = main_slice[subslice_idx : subslice_idx+subslice_size]
                f.write(''.join(subslice) + ',')
