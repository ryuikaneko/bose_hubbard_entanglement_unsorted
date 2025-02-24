#!/bin/bash

rsync -avz -e ssh dp11:/home/kaneko/work/entanglement_exact/ ./entanglement_exact/
rsync -avz -e ssh dp11:/home/kaneko/work/entanglement_random_sampling/ ./entanglement_random_sampling/

