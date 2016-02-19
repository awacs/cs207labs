import os
import itertools
import reprlib
from collections import defaultdict
def lc_reader(filename):
    lclist=[]
    i=0
    with open(filename) as fp:
        for line in fp:
            if i==0:
                keylist=line[1:-1].split()
                i+=1
            elif i==1:
                valuelist=line[1:-1].split()
                facetdict = dict(zip(keylist, valuelist))
                i+=1
            else:
                if line.find('#')!=0:
                    lclist.append([float(f) for f in line.strip().split()])
                
    return facetdict,lclist


class LightCurve:
    
    def __init__(self, data, metadict):
        self._time = [x[0] for x in data]
        self._amplitude = [x[1] for x in data]
        self._error = [x[2] for x in data]
        self.metadata = metadict
        self.filename = None
    
    @classmethod
    def from_file(cls, filename):
        lclist, metadict = lc_reader(filename)
        instance = cls(lclist, metadict)
        instance.filename = filename
        return instance
    
    def __repr__(self):
        class_name = type(self).__name__
        components = reprlib.repr(list(itertools.islice(self.timeseries,0,10)))
        components = components[components.find('['):]
        return '{}({})'.format(class_name, components)        
    @property
    def time(self):
        return self._time
    @property 
    def amplitude(self):
        return self._amplitude
    @property
    def timeseries(self):
        return zip(self._time,self._amplitude)
    def __len__(self):
        return len(list(self.timeseries))
    def __getitem__(self,position):
        return list(self.timeseries)[position]
    


class LightCurveDB:
    
    def __init__(self):
        self._collection={}
        self._field_index=defaultdict(list)
        self._tile_index=defaultdict(list)
        self._color_index=defaultdict(list)
    
    def populate(self, folder):
        for root,dirs,files in os.walk(folder): # DEPTH 1 ONLY
            for file in files:
                if file.find('.mjd')!=-1:
                    the_path = root+"/"+file
                    self._collection[file] = LightCurve.from_file(the_path)
    
    def index(self):
        for f in self._collection:
            lc, tilestring, seq, color, _ = f.split('.')
            field = int(lc.split('_')[-1])
            tile = int(tilestring)
            self._field_index[field].append(self._collection[f])
            self._tile_index[tile].append(self._collection[f])
            self._color_index[color].append(self._collection[f])
    def retrieve(self, facet, value):
        return getattr(self, '_'+facet+'_index')[value]