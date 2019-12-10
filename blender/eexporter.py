import os
import xml.etree.ElementTree as ET
import bpy

def should_add(obj):
    return obj.rigid_body != None or obj.get('components')

#rename meshes to match object names
for ob in bpy.data.objects:
    material_slots = ob.material_slots.keys()
    # rename hitboxes (only those that have not been renamed yet)
    if 'hitbox' in material_slots and 'hitbox' not in ob.name:
        ob.name += '_hitbox'
    ob.data.name = ob.name

data = ET.Element('data')

for obj in bpy.data.objects:
    # skip hitbox objects
    if '_hitbox' not in obj.name and should_add(obj):
        xml_object = ET.SubElement(data, 'object')
        xml_object.set('name', obj.name)

        if obj.rigid_body != None:
            # physics
            physics_obj = ET.SubElement(xml_object, 'physics')
            
            state_obj = ET.SubElement(physics_obj, 'state')
            state_obj.text = obj.rigid_body.type

            mass_obj = ET.SubElement(physics_obj, 'mass')
            mass_obj.text ='0' if obj.rigid_body.type == 'PASSIVE' else str(obj.rigid_body.mass)
            
            friction_obj = ET.SubElement(physics_obj, 'friction')
            friction_obj.text = str(obj.rigid_body.friction)
            
            restitution_obj = ET.SubElement(physics_obj, 'restitution')
            restitution_obj.text = str(obj.rigid_body.restitution)

        # components
        components_str = obj.get('components')
        if components_str:
            components_obj = ET.SubElement(xml_object, 'components')
            components = components_str.split('|') if components_str else []
            for component in components:
                component_obj = ET.SubElement(components_obj, component.capitalize())
                component_obj.text = '1'

str_data = ET.tostring(data)
exit_file = open(os.path.join(os.path.expanduser('~'), 'Downloads/data.xml'), 'wb')
exit_file.write(str_data)
exit_file.close()